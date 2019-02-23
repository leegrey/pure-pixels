/// @description MoveAndCollideOnGrid(grid)
/// Relies on instance variables: velocityX, velocityY, grounded

/// this version of MoveAndCollide performs continuous checking from start to finish
/// to avoid teleporting through objects.

var grid = argument0;

var signVelocityX = sign(velocityX);
var signVelocityY = sign(velocityY);

// perform a continuous collision along entire length of x

// fall back to the start of the pixel, (away from the direction of movement)
x = signVelocityX > 0 ? floor(x) : ceil(x);

// figure out how many steps to move
var xSteps = floor(abs(velocityX));

// note how there is a <= which means it is going to advance
// by a whole pixel even if velocity is lower than a whole pixel
// (which is not ideal)

for(var ix = 0; ix <= xSteps; ix++)
{
	// if the next step is a collision, break
	if (GridCollide(signVelocityX, 0, grid))
	{
		velocityX = 0;
		break;
	}
	// otherwise apply the movement and continue
	x += signVelocityX;		
}

// reset grounded flag
grounded = false;

// perform a continuous collision along entire length of y
y = signVelocityY > 0 ? floor(y) : ceil(y);
	
// figure out how many steps to move
var ySteps = floor(abs(velocityY));
for(var iy = 0; iy <= ySteps; iy++)
{
	// if the next step is a collision, break
	if (GridCollide(0, signVelocityY, grid))
	{
		// if we are moving downward on Y then we hit the ground;
		
		if (signVelocityY > 0) grounded = true;
	
		velocityY = 0;
		break;
	}
	
	// otherwise apply the movement and continue
	y += signVelocityY;		
}
