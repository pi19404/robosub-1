## directive
    * When the ai says "I want to move forward", the directive layer should
       handle that command.
    * Commands out of this layer will state the fuzzy set membership of the
       robosub. For example, if the sub needs to move five inches to the right
       and rotate slightly to the left, then directive layer will pass on
       commands that looks like is_too_far_left(0.01) and is_rotated_right(0.1).
       * The syntax is in this format so that it is easily differentiated from
          the stabilization layer.

## stabalization
    * This layer will accept commands from the directive layer, and it will
       accomidate those commands if it can do so while satisfying some constraints.
       * One constraint is that we keep a flat orientation. If we descent, this
          layer should make sure we do not tilt while doing so.
       * If we send a command and a slighly different command is executed (for
          example, because current is making us strafe) then this layer should
          recognize that and modify its raw commands to compensate.
       * In general, this layer should be computing a change of basis before it
          sends the commands to the physical layer.
    * This layer will express commands using 6 floats. The domain of all values
       is between -1 and +1.
       * move.x, move.y, move.z, rotate.x_axis, rotate.y_axis, rotate.z_axis
    * Syntactally, the commands will be of the form "The sub should do this".

## physical layer

* All raw movement commands pass through the physical layer
* At its most basic form, the physical layer needs to respond to each of the
   six movement commands from the stabalization layer in a linearly independent
   way. Hopefully the command move.x => 1.0 will translate to moving forward
   rather than doing a barrel roll, but hopefully the stabalization layer will
   apply a linear transformation to translate what it wants to happen to what
   actually does happen.

