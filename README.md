# Scripts-Data-Exposer-FS
Adds functions to HKS that let you access more data about the game to use in your scripts.

See examples/tutorials in NewHksInfo.lua

This fork adds some more functionality in a slightly more accessible API. Currently, allows teleport to target, and teleporting target to player.

## Extensions
In your HKS, the following APIs are available:

### Teleport Player to Target
    -- Teleport to target
    act(50000, 1)

### Teleport Target to Player
    -- Teleport target to player
    act(50000, 2)

### Teleport Player to Next Target

You can select upto 5 targets by simply locking on to enemies consecutively. This skips over dead targets or targets on a different map.

    -- Teleport target to player
    act(50000, 3)

### Distance to Target
    
    local distanceToTarget = IntBitsToFloat(env(50000, 4))

### Modify RootMotionReduction factor

This allows for dynamically modifying this TAE parameter from HKS. Currently, accepts a single value which is set to both *start* and *end* factors.

    -- set RootMotionReduction factor to 0.5
    act(50002, 500)

Values can range from 0 to 1000, which are mapped to [0, 1].

More functionality may be added as per requirements.
