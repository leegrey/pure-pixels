/// @description GridCollide(offsetX, offsetY, grid)
/// @param offsetX - a proposed vertical movement from the current x position
/// @param offsetY - a proposed horizontal movement from the current y position

var offsetX = argument0;
var offsetY = argument1;
var grid = argument2;

var tileSize = oTileWorld.tileSize;

var left = bbox_left + offsetX;
var right = bbox_right + offsetX;
var top = bbox_top + offsetY;
var bottom = bbox_bottom + offsetY;

var minX = floor(left / tileSize);
var maxX = floor(right / tileSize);
var minY = floor(top / tileSize);
var maxY = floor(bottom / tileSize);

var mapWidth = oTileWorld.mapWidth;
var mapHeight = oTileWorld.mapHeight;

// scan from min and max for any overlaps
var isColliding = false;

for(var iy = minY; iy <= maxY; iy++)
{
	if (isColliding) break;
	for(var ix = minX; ix <= maxX; ix++)
	{
		// todo: make use of the fact we actually know which tiles were hit				
		if (ix < 0 || iy < 0 || ix >= mapWidth || iy >= mapHeight ) continue;
		
		var overlapTile = grid[# ix, iy];
		
		var isSolid = (overlapTile >> mSolidBitShift) & 1;
		
		if (isSolid)
		{
			isColliding = true;
			break;
		}
	}
}

return isColliding;


