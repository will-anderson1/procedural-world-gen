### Prereqs
- SDL3 (installable via brew)
- glm (installable via brew)
- OpenGL (comes with macos)

### Building
`cmake -B build -S . && cmake --build build`

### Executing
`./build/ProcedualWorldGen`

### Controls
- Move with arrow keys
- Pan with mouse
- `c`: toggle color/texture mode
- `m`: toggle movement mode (ground or freecam)
- `w`: increase speed
- `s`: decrease speed
- `f`: fullscreen toggle
- `esc`: windowed mode
