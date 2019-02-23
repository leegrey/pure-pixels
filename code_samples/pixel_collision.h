#pragma once

namespace lg {
        
    bool moveAndCollideOnPixels(PixelWorld& world, PixelEntity& entity, bool climbSlopes = false) {

        bool wasGrounded = entity.grounded;

        Vector2 originalVelocity = entity.velocity;
        Vector2 originalPos = entity.position;
        
        // calculate the rectangle of the entity in the world
        // (snap to pixel / integer representation)
        SDL_Rect worldRect = entity.rect;
        worldRect.x += floor(entity.position.x);
        worldRect.y += floor(entity.position.y);
        
        // make a copy for projecting along the velocity vector
        SDL_Rect projectedRect = worldRect;

        int signVelocityX = sign(entity.velocity.x);
        int signVelocityY = sign(entity.velocity.y);

        // perform a continuous collision along entire length of x
        // fall back to the start of the pixel, (away from the direction of movement)

        // Calculate steps to test. This takes into account floating point velocity, and 
        // determines if crossing a boundary or not. 
        // (eg if moving from 10.2 to 10.3, no boundary is crossed so no projection is needed)

        int xSteps = 
            abs(
                floorToInt(entity.position.x + entity.velocity.x)
                - floorToInt(entity.position.x)
            );   

        // PROOF:
        // 10.35  => 10.45
        //  floorToInt( 10.35 + 0.1f)  // 10
        //  - floorToInt(10.35)        // 10

        // 10.95  => 11.05
        //  floorToInt( 10.95 + 0.1f)  // 11
        //  - floorToInt(10.95)        // 10
                // 11 - 10 = 1
        //      steps: 1 

        bool hitX = false;
        bool hitY = false;
        
        // Start with a rect of the current position
        // Move it one pixel at a time along x, stop if it hits something
        // then repeat for y. 

        int climbSlopesCount = 0;

        // keep track of last good x
        int goodX = projectedRect.x;

        for (int ix = 0; ix < xSteps; ix++)
        {
            // advance one pixel at a time in the dxirection of velocity.x
            // (add sign, so there is an offset even when ix is zero )
            projectedRect.x += signVelocityX;

            // (test collision of projected rect)
            if (isColliding(world, projectedRect)) {
                
                // Only actually calculate it if climbSlopes is true
                // (reset flag)
                bool isCollidingAbove = false;

                // only climb slopes if the flag is true
                if (climbSlopes) {
                    // test with projected rect, and a one pixel vertical offset
                    isCollidingAbove = isCollidingAtOffset(world, projectedRect, 0, -1);
                }
                
                // if climb flag is true, and there is no collision one pixel up...
                if (climbSlopes && !isCollidingAbove) {
                    // if the area is clear above, just move move the rec up by one pixel
                    projectedRect.y -= 1;
                
                    // for debug only
                    climbSlopesCount++;
                    
                    // do nothing to X at all, but mark hitY as true
                    // we need to do this so y value of projected rect is applied at the ends
                    hitY = true; 

                } else {
                    // cancel x velocity because we hit something
                    entity.velocity.x = 0;
                    // reset x to the last good x
                    projectedRect.x = goodX;
                
                    hitX = true;
                    
                    break;
                }
            }
            goodX = projectedRect.x;  
        }

        // reset grounded flag
        entity.grounded = false;
        
        // figure out how many steps to move
        int ySteps = 
            abs(floorToInt(entity.position.y + entity.velocity.y) 
            - floorToInt(entity.position.y));

        // remember the last good position on y
        int goodY = projectedRect.y;
        
        for(int iy = 0; iy < ySteps; iy++) {
            projectedRect.y += signVelocityY;
            // if the next step is a collision, break
            if (isColliding(world, projectedRect))
            {
                // if we are moving downward on Y then we hit the ground
                // NOTE: this is redundant if we do the additional grounded test below
                if (signVelocityY > 0) entity.grounded = true;

                // cancel y velocity because we hit something 
                entity.velocity.y = 0;

                // reset y to the last good position
                projectedRect.y = goodY;
                hitY = true;
                break;
            }
        }

        // apply the translation to our position
        if (hitX) {
            // NOTE: pull pos directly from projected Rect integers, ie `=` not `+=` 
            entity.position.x = projectedRect.x - entity.rect.x;
        } else {
            // if we didn't hit, just apply floating point velocity to preserve
            // smooth movement and acceleration... 
            /// (ie, avoid snapping to a pixel unless we need to!)
            entity.position.x += entity.velocity.x;
        }
        
        if (hitY) {
            // NOTE: pull pos directly from projected Rect integers, ie `=` not `+=` 
            entity.position.y = projectedRect.y - entity.rect.y;
        } else {
            entity.position.y += entity.velocity.y;
        }
        
        // check one pixel down for ground
        entity.grounded = isCollidingAtOffset(world, projectedRect, 0, 1);

        // return a bool to indicate if we hit anything
        return hitX || hitY;
    }

}