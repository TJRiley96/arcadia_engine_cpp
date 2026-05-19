#version 330 core
  uniform sampler2D texture1;
  uniform sampler2D texture2;

  in vec4 frag_color;
  in vec2 frag_tex;

  out vec4 final_color;

  void main() {
    final_color = mix(texture(logo_texture, frag_tex), texture(garris_texture, frag_tex), 0.4);
  }