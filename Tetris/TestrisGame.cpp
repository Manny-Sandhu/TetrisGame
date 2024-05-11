#include "TetrisGame.h"
#include <iostream> 
#include <cassert>


const int TetrisGame::BLOCK_WIDTH{ 32 };
const int TetrisGame::BLOCK_HEIGHT{ 32 };
const double TetrisGame::MAX_SECONDS_PER_TICK{ 0.72 };
const double TetrisGame::MIN_SECONDS_PER_TICK{ 0.72 };
const double TetrisGame::MAX_LEVELS{ 100.0 };
// constructor
//   initialize/assign private member vars names that match param names
//   reset() the game
//   load font from file: fonts/RedOctober.ttf
//   setup scoreText
// - params: already specified
TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
	:window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset)
{
	// setup our fonts for drawing the score
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf")) { assert(false && "Missing font: RedOctober.ttf"); };
	if (!levelFont.loadFromFile("fonts/RedOctober.ttf")) { assert(false && "Missing font: RedOctober.ttf"); };
	if (!gameOverFont.loadFromFile("fonts/RedOctober.ttf")) { assert(false && "Missing font: RedOctober.ttf"); };

	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);

	levelText.setFont(levelFont);
	levelText.setCharacterSize(18);
	levelText.setFillColor(sf::Color::White);
	levelText.setPosition(425, 352);

	gameOverText.setFont(gameOverFont);
	gameOverText.setCharacterSize(18);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setPosition(58, 352);

	// setup our game sounds
	if (!musicBuffer.loadFromFile("sounds/tetrisMusic.ogg")) { assert(false && "Missing sound: tetrisMusic.ogg"); };
	if (!dropBuffer.loadFromFile("sounds/blockDrop.ogg")) { assert(false && "Missing sound: blockDrop.ogg"); };
	if (!rotateBuffer.loadFromFile("sounds/blockRotate.ogg")) { assert(false && "Missing sound: blockRotate.ogg"); };
	if (!levelUpBuffer.loadFromFile("sounds/levelUp.ogg")) { assert(false && "Missing sound: levelUp.ogg"); };
	if (!gameOverBuffer.loadFromFile("sounds/gameOver.ogg")) { assert(false && "Missing sound: gameOver.ogg"); };
	if (!clearedBuffer.loadFromFile("sounds/cleared.mp3")) { assert(false && "Missing sound: cleared.mp3"); };

	music.setBuffer(musicBuffer);
	music.setLoop(true);
	music.setVolume(10);

	dropSound.setBuffer(dropBuffer);
	rotateSound.setBuffer(rotateBuffer);
	levelUp.setBuffer(levelUpBuffer);
	gameOverSound.setBuffer(gameOverBuffer);
	clearedSound.setBuffer(clearedBuffer);
	
	reset();
}

// Draw anything to do with the game,
//   includes the board, currentShape, nextShape, score
//   called every game loop
// - params: none
// - return: nothing
void TetrisGame::draw()
{
	drawGameboard();
	window.draw(scoreText);
	window.draw(levelText);
	window.draw(gameOverText);
	drawTetromino(currentShape, gameboardOffset, 255);
	drawTetromino(nextShape, nextShapeOffset, 255);
	drawTetromino(ghostShape, gameboardOffset, 128);
}

// Event and game loop processing
// handles keypress events (up, left, right, down, space)
// - param 1: sf::Event event
// - return: nothing
void TetrisGame::onKeyPressed(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			if (attemptRotate(currentShape))
			{ 
				rotateSound.play();
				updateGhostShape();
			}
		}
		if (event.key.code == sf::Keyboard::Left)
		{
			attemptMove(currentShape,-1,0);
			updateGhostShape();
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			attemptMove(currentShape, 1, 0);
			updateGhostShape();
		}
		if (event.key.code == sf::Keyboard::Down)
		{
			attemptMove(currentShape, 0, 1);
			updateGhostShape();
		}
		if (event.key.code == sf::Keyboard::Space)
		{
			drop(currentShape);
			lock(currentShape);
			dropSound.play();
		}
		if (event.key.code == sf::Keyboard::Q)
		{
			if (gameOver) { window.close(); }
		}
		if (event.key.code == sf::Keyboard::Enter)
		{
			if (gameOver) { reset(); }
		}
	}
}

// called every game loop to handle ticks & tetromino placement (locking)
// - param 1: float secondsSinceLastLoop
// return: nothing
void TetrisGame::processGameLoop(float secondsSinceLastLoop)
{
	secondsSinceLastTick += secondsSinceLastLoop;
	if (secondsSinceLastTick > secondsPerTick)
	{
		tick(); 
		secondsSinceLastTick = 0.0;
	}
	if (shapePlacedSinceLastGameLoop) 
	{
		if (spawnNextShape()) 
		{
			nextShape.setShape(Tetromino::getRandomShape());

			int rowsRemoved = board.removeCompletedRows();
			if (rowsRemoved > 0)
			{
				clearedSound.play();
			}

			if (rowsRemoved == 1)
			{
				score += 40 * (level + 1);
				determineSecondsPerTick();
			}
			if (rowsRemoved == 2)
			{
				score += 100 * (level + 1);
				determineSecondsPerTick();
			}
			if (rowsRemoved == 3)
			{
				score += 300 * (level + 1);
				determineSecondsPerTick();
			}
			if (rowsRemoved == 4)
			{
				score += 1200 * (level + 1);
				determineSecondsPerTick();
			}
			updateScoreDisplay();
			cleared += rowsRemoved;
		
			if (cleared >= 10)
			{
				level++;
				cleared = cleared % 10;
				levelUp.play();
				updateLevelDisplay();
			}
		}
		else 
		{
			gameOver = true;
			music.stop();
			gameOverSound.play();
			gameOverText.setString("Play Again: Enter   Quit: Q");
		}

		shapePlacedSinceLastGameLoop = false;
	}
}

// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further).
// - params: none
// - return: nothing
void TetrisGame::tick()
{
	bool check = attemptMove(currentShape, 0, 1);
	updateGhostShape();
	if (!check) 
	{
		lock(currentShape);
	}
}

// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
// - params: none
// - return: nothing
void TetrisGame::reset()
{
	score = 0;
	level = 0;
	updateScoreDisplay();
	updateLevelDisplay();
	determineSecondsPerTick();
	board.empty();
	pickNextShape();
	spawnNextShape();
	pickNextShape();
	music.play();
	gameOver = false;
}

// assign nextShape.setShape a new random shape  
// - params: none
// - return: nothing
void TetrisGame::pickNextShape()
{
	nextShape.setShape(Tetromino::getRandomShape());
}

// update the ghost shape to drop just under the current shape
// - params: nothing
// - return: nothing
void TetrisGame::updateGhostShape()
{
	ghostShape = currentShape;
	drop(ghostShape);
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
// - params: none
// - return: bool, true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape()
{
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());
	if (isPositionLegal(currentShape))
	{
		return true;
	}
	return false;
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To accomplish this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (temp.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
// - param 1: GridTetromino shape
// - return: bool, true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape)
{
	if (gameOver) { return false; }
	GridTetromino temp = shape;
	temp.rotateClockwise();
	if (isPositionLegal(temp))
	{
		shape.rotateClockwise();
		return true;
	}
	return false;
}

// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.	
// - param 1: GridTetromino shape
// - param 2: int x;
// - param 3: int y;
// - return: true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y)
{
	if (gameOver) { return false; }
	GridTetromino temp = shape;
	temp.move(x,y);
	if (isPositionLegal(temp)) 
	{
		shape.move(x, y);
		return true;
	}
	return false;
}

// drops the tetromino vertically as far as it can 
//   legally go.  Use attemptMove(). This can be done in 1 line.
// - param 1: GridTetromino shape
// - return: nothing;
void TetrisGame::drop(GridTetromino& shape)
{
	while (attemptMove(shape, 0, 1)){}
}

// copy the contents (color) of the tetromino's mapped block locs to the grid.
	//	 1) get the tetromino's mapped locs via tetromino.getBlockLocsMappedToGrid()
	//   2) use the board's setContent() method to set the content at the mapped locations.
	//   3) record the fact that we placed a shape by setting shapePlacedSinceLastGameLoop
	//      to true
	// - param 1: GridTetromino shape
	// - return: nothing
void TetrisGame::lock(GridTetromino& shape)
{
	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();
	for (auto& block : blocks)
	{
		board.setContent(block, shape.getColor());
	}
	shapePlacedSinceLastGameLoop = true;
}

bool TetrisGame::getGameOver() const
{
	return gameOver;
}
// Graphics methods ==============================================

// Draw a tetris block sprite on the canvas		
// The block position is specified in terms of 2 offsets: 
//    1) the top left (of the gameboard in pixels)
//    2) an x & y offset into the gameboard - in blocks (not pixels)
//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
//       to get the pixel offset.
//	 1) set the block color using blockSprite.setTextureRect()
//   2) set the block location using blockSprite.setPosition()   
//	 3) draw the block using window.draw()
//   For details/instructions on these 3 operations see:
//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
//       use member variables: window and blockSprite (assigned in constructor)
// param 1: Point topLeft
// param 2: int xOffset
// param 3: int yOffset
// param 4: TetColor color
// return: nothing
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, const TetColor& color, int transparency)
{
	blockSprite.setTextureRect(sf::IntRect(static_cast<int>(color) * 32, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setColor(sf::Color(255, 255, 255, transparency));
	blockSprite.setPosition(topLeft.getX() + BLOCK_WIDTH * xOffset, topLeft.getY() + BLOCK_WIDTH * yOffset);
	window.draw(blockSprite);
}

// Draw the gameboard blocks on the window
//   Iterate through each row & col, use drawBlock() to 
//   draw a block if it isn't empty.
// params: none
// return: nothing
void TetrisGame::drawGameboard()
{
	for (int i = 0; i < board.MAX_Y; i++)
	{
		for (int j = 0; j < board.MAX_X; j++)
		{
			if (board.getContent(j, i) != Gameboard::EMPTY_BLOCK)
			{
				TetColor color = static_cast<TetColor>(board.getContent(j, i));
				drawBlock(gameboardOffset, j, i, color, 255);
			}
		}
	}
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
// param 1: GridTetromino tetromino
// param 2: Point topLeft
// return: nothing
void TetrisGame::drawTetromino(GridTetromino& tetromino, Point topLeft, int transparency)
{
	std::vector<Point>blocks = tetromino.getBlockLocsMappedToGrid();
	
	for (auto& block : blocks)
	{
		drawBlock(topLeft, block.getX(), block.getY(), tetromino.getColor(), transparency);
	}
}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
// params: none:
// return: nothing
void TetrisGame::updateScoreDisplay() 
{
	std::string current_score = std::to_string(score);
	std::string text = "score: " + current_score;
	scoreText.setString(text);
}

// update the level display
// form a string "level: ##" to display the current level
// user levelText.setString() to display it.
// params: none:
// return: nothing
void TetrisGame::updateLevelDisplay()
{
	std::string current_level = std::to_string(level);
	std::string text = "level: " + current_level;
	levelText.setString(text);
}

// State & gameplay/logic methods ================================

// Determine if a Tetromino can legally be placed at its current position
// on the gameboard.
//   Tip: Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
// - param 1: GridTetromino shape
// - return: bool, true if shape is within borders (isWithinBorders()) and 
//           the shape's mapped board locs are empty (false otherwise).
bool TetrisGame::isPositionLegal(GridTetromino& shape) const
{
	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid(); 
	if (board.areAllLocsEmpty(blocks) && isWithinBorders(shape))
	{
		return true;
	}
	return false;
}


// Determine if the shape is within the left, right, & bottom gameboard borders
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
// - param 1: GridTetromino shape
// - return: bool, true if the shape is within the left, right, and lower border
//	         of the grid, but *NOT* the top border (false otherwise)
bool TetrisGame::isWithinBorders(GridTetromino shape) const
{
	std::vector<Point> blocks = shape.getBlockLocsMappedToGrid();
	bool check = true;
	for (auto& block : blocks)
	{
		if (block.getX() >= 0 && block.getX() <= 9 && block.getY() <= Gameboard::MAX_Y){} // do nothing if this is true
		else { check = false; }
	}
	return check;
}


// set secsPerTick 
//   - basic: use MAX_SECS_PER_TICK
//   - advanced: base it on score (higher score results in lower secsPerTick)
// params: none
// return: nothing
void TetrisGame::determineSecondsPerTick()
{
	double update_seconds = MAX_SECONDS_PER_TICK - (static_cast<double>(level)/ MAX_LEVELS);		// max levels are 10 and 300 points bumps you to a new level for now
	secondsPerTick = update_seconds;
}