# CutePlane 3D
 A simple endless runner game featuring a plane flying over the ocean, constantly needing fuel to continue it's journey, while having to dodge incoming obstacles.

![](Resources/Gameplay1.gif)

#### - Project Status: [Completed]

### Built with
  * C++
  * [OpenGL](https://www.opengl.org/)
  * [EGC-Framework](https://github.com/UPB-Graphics/Framework-EGC)
  
### More examples

![](Resources/Gameplay2.gif)

#### Lighting
The lighting is based on the Phong reflection model, and most object have their own fragment shader to make them look just right.

The water, obstacles and fuel are moved directly from vertex shader, using a pseudo-random function that uses the vertex **position**
and current time to give off a chaotic feel.

The plane moves with inertia, acceleration speeds up the propeller and the plane start following the mouse.

Collisions result in a radial impact from the collision point, sending the plane back, temporarily "stunning" it, with a loss of control.

If the plane runs out of fuel, or gets hit more than trice, it'll crash into the ocean.

## Usage
The plane follows the `mouse` movement, try to avoid obstacles and grab fuel for as long as you can.
## Completion Date
2019, Novemeber 28
