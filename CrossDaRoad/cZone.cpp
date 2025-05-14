#include "cZone.h"
#include <cstring>
#include <iostream>

/**
 * @file cZone.cpp
 *
 * @brief Contains zone class implementation
 *
 * This file implements zone class for zone management.
**/

////////////////////////////////////////////////////////////////////////
////////////////////////// CONSTRUCTORS AND DESTRUCTOR /////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Default constructor
cZone::cZone()
{
	nZoneWidth = 0;
	nZoneHeight = 0;
	bDangers = nullptr;
	bBlocks = nullptr;
}
/// @brief Parameterized constructor
/// @param nWidth width of the zone
/// @param nHeight height of the zone
cZone::cZone(const int nWidth, const int nHeight)
{
	CreateZone(nWidth, nHeight);
}
/// @brief Destructor
cZone::~cZone()
{
	if (bDangers != nullptr) {
		delete[] bDangers;
		bDangers = nullptr;
	}
	if (bBlocks != nullptr) {
		delete[] bBlocks;
		bBlocks = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////
/////////////////// CONSTRUCTOR FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Create zone with size nWidth x nHeight, set danger and block pixels
/// @param nWidth width of the zone
/// @param nHeight height of the zone
/// @param bDanger value to set danger pixels (true: danger, false: safe)
/// @param bBlock value to set block pixels (true: block, false: unblock)
/// @return true if successfully created zone, false otherwise
bool cZone::CreateZone(const int nWidth, const int nHeight, const bool bDanger, const bool bBlock)
{
	if (nWidth <= 0 || nHeight <= 0) {
		std::cerr << "In cZone.cpp, calling bool cZone::CreateZone(int width = "
			<< nWidth << ", int height = " << nHeight
			<< ") is invalid, expected positive integer parameters";
		return false;
	}
	nZoneWidth = nWidth;
	nZoneHeight = nHeight;
	bDangers = new bool[nZoneWidth * nZoneHeight];
	bBlocks = new bool[nZoneWidth * nZoneHeight];
	memset(bDangers, bDanger, nZoneWidth * nZoneHeight * sizeof(bool));
	memset(bBlocks, bBlock, nZoneWidth * nZoneHeight * sizeof(bool));
	return true;
}

/// @brief Create zone with size nWidth x nHeight, set danger and block pixels to safe and unblock
bool cZone::CreateZone(const int nWidth, const int nHeight)
{
	return CreateZone(nWidth, nHeight, false, false);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////// CHECKERS ////////////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Check if graphic is danger
/// @param graphic graphic to check 
/// @param danger_pattern danger pattern to check 
/// @return true if graphic is danger, false otherwise
bool cZone::IsDanger(const char& graphic, const char* danger_pattern) const
{
	return strchr(danger_pattern, graphic) != nullptr;
}
/// @brief Check if graphic is safe
/// @param graphic graphic to check
/// @param danger_pattern danger pattern to check
/// @return true if graphic is safe, false otherwise
bool cZone::IsSafe(const char& graphic, const char* danger_pattern) const
{
	return !IsDanger(graphic, danger_pattern);
}
/// @brief Check if graphic is blocked
/// @param graphic graphic to check
/// @param block_pattern block pattern to check
/// @return true if graphic is blocked, false otherwise
bool cZone::IsBlocked(const char& graphic, const char* block_pattern) const
{
	return strchr(block_pattern, graphic) != nullptr;
}
/// @brief Check if graphic is unblocked
/// @param graphic graphic to check
/// @param block_pattern block pattern to check
bool cZone::IsUnblocked(const char& graphic, const char* block_pattern) const
{
	return !IsBlocked(graphic, block_pattern);
}
/// @brief Check if (x, y) is inside the zone
/// @param x x coordinate 
/// @param y y coordinate
/// @return true if (x, y) is inside the zone, false otherwise
bool cZone::IsInside(const int x, const int y) const
{
	return (x >= 0 && x < nZoneWidth) && (y >= 0 && y < nZoneHeight);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////// SETTERS /////////////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Set danger pixel at a position
/// @param nPosX x coordinate
/// @param nPosY y coordinate
/// @param bValue value to set (true: danger, false: safe)
/// @return true if successfully set danger pixel (if (nPosX, nPosY) is inside), false otherwise
bool cZone::SetDanger(const int nPosX, const int nPosY, const bool bValue) const
{
	if (!IsInside(nPosX, nPosY)) {
		return false;
	}
	bDangers[nPosY * nZoneWidth + nPosX] = bValue;
	return true;
}
/// @brief Set block pixel at a position
/// @param nPosX x coordinate
/// @param nPosY y coordinate
/// @param bValue value to set (true: block, false: unblock)
/// @return true if successfully set block pixel (if (nPosX, nPosY) is inside), false otherwise
bool cZone::SetBlock(const int nPosX, const int nPosY, const bool bValue) const
{
	if (!IsInside(nPosX, nPosY)) {
		return false;
	}
	bBlocks[nPosY * nZoneWidth + nPosX] = bValue;
	return true;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////// FILLERS /////////////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Fill danger pixels with graphic in the zone
/// @param nTopLeftX top left x coordinate
/// @param nTopLeftY top left y coordinate
/// @param nBottomRightX bottom right x coordinate
/// @param nBottomRightY bottom right y coordinate
/// @param graphic graphic to fill
/// @param danger_pattern danger pattern to check if graphic is danger or not
/// @return number of danger pixels filled
int cZone::FillDanger(const int nTopLeftX, const int nTopLeftY, const int nBottomRightX, const int nBottomRightY, const char& graphic, const char* danger_pattern) const
{
	int counter = 0;
	for (int x = nTopLeftX; x < nBottomRightX; x++) {
		for (int y = nTopLeftY; y < nBottomRightY; y++) {
			counter += SetDanger(x, y, IsDanger(graphic, danger_pattern));
		}
	}
	return counter;
}
/// @brief  Fill safe pixels with graphic in the zone
/// @param nTopLeftX x coordinate of top left corner
/// @param nTopLeftY y coordinate of top left corner
/// @param nBottomRightX x coordinate of bottom right corner
/// @param nBottomRightY y coordinate of bottom right corner
/// @param graphic graphic to fill
/// @param danger_pattern danger pattern to check if graphic is danger or not
/// @return number of safe pixels filled
int cZone::FillSafe(const int nTopLeftX, const int nTopLeftY, const int nBottomRightX, const int nBottomRightY, const char& graphic, const char* danger_pattern) const
{
	int counter = 0;
	for (int x = nTopLeftX; x < nBottomRightX; x++) {
		for (int y = nTopLeftY; y < nBottomRightY; y++) {
			counter += SetDanger(x, y, IsSafe(graphic, danger_pattern));
		}
	}
	return counter;
}
/// @brief Fill block pixels with graphic in the zone
/// @param nTopLeftX top left x coordinate
/// @param nTopLeftY top left y coordinate
/// @param nBottomRightX bottom right x coordinate
/// @param nBottomRightY bottom right y coordinate
/// @param graphic graphic to fill
/// @param block_pattern block pattern to check if graphic is block or not
/// @return number of block pixels filled
int cZone::FillBlocked(const int nTopLeftX, const int nTopLeftY, const int nBottomRightX, const int nBottomRightY, const char& graphic, const char* block_pattern) const
{
	int counter = 0;
	for (int x = nTopLeftX; x < nBottomRightX; x++) {
		for (int y = nTopLeftY; y < nBottomRightY; y++) {
			counter += SetBlock(x, y, IsBlocked(graphic, block_pattern));
		}
	}
	return counter;
}
/// @brief Fill unblock pixels with graphic in the zone
/// @param nTopLeftX top left x coordinate
/// @param nTopLeftY top left y coordinate
/// @param nBottomRightX bottom right x coordinate
/// @param nBottomRightY bottom right y coordinate
/// @param graphic graphic to fill
/// @param block_pattern block pattern to check if graphic is block or not
/// @return number of unblock pixels filled
int cZone::FillUnblocked(int nTopLeftX, int nTopLeftY, int nBottomRightX, int nBottomRightY, const char& graphic, const char* block_pattern) const
{
	int counter = 0;
	for (int x = nTopLeftX; x < nBottomRightX; x++) {
		for (int y = nTopLeftY; y < nBottomRightY; y++) {
			counter += SetBlock(x, y, IsUnblocked(graphic, block_pattern));
		}
	}
	return counter;
}

////////////////////////////////////////////////////////////////////////
//////////////////////// DANGER ZONE CHECKERS //////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Check if pixel at (x, y) is danger
/// @param x x coordinate
/// @param y y coordinate
/// @return true if pixel at (x, y) is danger, false otherwise
bool cZone::IsDangerPixel(const float x, const float y) const
{
	const bool isDangerPixel = bDangers[static_cast<int>(y) * nZoneWidth + static_cast<int>(x)];
	return isDangerPixel;
}

/// @brief Check if pixel at (x, y) is danger (top left) 
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if top left pixel at (x, y) is danger, false otherwise
bool cZone::IsDangerTopLeft(const float x, const float y, const int size) const
{
	const bool isDangerTopLeft = IsDangerPixel(x * static_cast<float>(size) + 1, y * static_cast<float>(size) + 1);
	return isDangerTopLeft;
}

/// @brief Check if pixel at (x, y) is danger (top right) 
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if top right pixel at (x, y) is danger, false otherwise
bool cZone::IsDangerTopRight(const float x, const float y, const int size) const
{
	const bool isDangerTopRight = IsDangerPixel((x + 1) * static_cast<float>(size) - 1, y * static_cast<float>(size) + 1);
	return isDangerTopRight;
}

/// @brief Check if pixel at (x, y) is danger (bottom left)
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if bottom left pixel at (x, y) is danger, false otherwise
bool cZone::IsDangerBottomLeft(const float x, const float y, const int size) const
{
	const bool isDangerBottomLeft = IsDangerPixel(x * static_cast<float>(size) + 1, (y + 1) * static_cast<float>(size) - 1);
	return isDangerBottomLeft;
}

/// @brief Check if pixel at (x, y) is danger (bottom right)
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if bottom right pixel at (x, y) is danger, false otherwise
bool cZone::IsDangerBottomRight(const float x, const float y, const int size) const
{
	const bool isDangerBottomRight =
		IsDangerPixel((x + 1) * static_cast<float>(size) - 1, (y + 1) * static_cast<float>(size) - 1);
	return isDangerBottomRight;
}

////////////////////////////////////////////////////////////////////////
//////////////////////// BLOCK ZONE CHECKERS ///////////////////////////
////////////////////////////////////////////////////////////////////////

/// @brief Check if pixel at (x, y) is block
/// @param x x coordinate
/// @param y y coordinate
/// @return true if pixel at (x, y) is block, false otherwise
bool cZone::IsBlockedPixel(const float x, const float y) const
{
	const bool isBlockPixel = bBlocks[static_cast<int>(y) * nZoneWidth + static_cast<int>(x)];
	return isBlockPixel;
}

/// @brief Check if pixel at (x, y) is block (top left) 
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if top left pixel at (x, y) is block, false otherwise
bool cZone::IsBlockedTopLeft(const float x, const float y, const int size) const
{
	const bool isBlockTopLeft = IsBlockedPixel(x * static_cast<float>(size) + 1, y * static_cast<float>(size) + 1);
	return isBlockTopLeft;
}

/// @brief Check if pixel at (x, y) is block (top right)
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if top right pixel at (x, y) is block, false otherwise
bool cZone::IsBlockedTopRight(const float x, const float y, const int size) const
{
	const bool isBlockTopRight = IsBlockedPixel((x + 1) * static_cast<float>(size) - 1, y * static_cast<float>(size) + 1);
	return isBlockTopRight;
}

/// @brief Check if pixel at (x, y) is block (bottom left)
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if bottom left pixel at (x, y) is block, false otherwise
bool cZone::IsBlockedBottomLeft(const float x, const float y, const int size) const
{
	const bool isBlockBottomLeft = IsBlockedPixel(x * static_cast<float>(size) + 1, (y + 1) * static_cast<float>(size) - 1);
	return isBlockBottomLeft;
}

/// @brief Check if pixel at (x, y) is block (bottom right)
/// @param x x coordinate
/// @param y y coordinate
/// @param size size of the pixel
/// @return true if bottom right pixel at (x, y) is block, false otherwise
bool cZone::IsBlockedBottomRight(const float x, const float y, const int size) const
{
	const bool isBlockBottomRight = IsBlockedPixel((x + 1) * static_cast<float>(size) - 1, (y + 1) * static_cast<float>(size) - 1);
	return isBlockBottomRight;
}
