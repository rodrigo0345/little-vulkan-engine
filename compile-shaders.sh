
mkdir -p shaders/build
/usr/local/bin/glslc shaders/simple_shader.vert -o shaders/build/simple_shader.vert.spv
/usr/local/bin/glslc shaders/simple_shader.frag -o shaders/build/simple_shader.frag.spv
