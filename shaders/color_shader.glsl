#version 410 core
out vec4 FragColor;

in float Height;
in vec2 TexCoord;
uniform bool textureMode;

uniform sampler2D oceanTex;
uniform sampler2D beachTex;
uniform sampler2D forestTex;
uniform sampler2D mountainTex;

uniform float waterHeight;
uniform float beachHeight;
uniform float grassHeight;
uniform float rockHeight;

void main()
{
    if (textureMode == true) {
        float h = (Height + 50.0) / 100.0;
        
        vec4 waterColor = texture(oceanTex, TexCoord);
        vec4 beachColor = texture(beachTex, TexCoord);
        vec4 grassColor = texture(forestTex, TexCoord);
        vec4 rockColor = texture(mountainTex, TexCoord);

        // https://www.gamedeveloper.com/programming/advanced-terrain-texture-splatting
        if (h < waterHeight) {
            FragColor = waterColor;
        } 
        else if (h < beachHeight) {
            FragColor = mix(waterColor, beachColor, (h - waterHeight) / (beachHeight - waterHeight));
        } 
        else if (h < grassHeight) {
            FragColor = mix(beachColor, grassColor, (h - beachHeight) / (grassHeight - beachHeight));
        } 
        else {
            FragColor = mix(grassColor, rockColor, (h - grassHeight) / (rockHeight - grassHeight));
        }
    } 
    else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        float h = (Height + 50.0) / 100.0;
        vec4 waterColor = vec4(0.0, 0.0, 0.3, 1.0);
        vec4 beachColor = vec4(0.3, 0.3, 0.0, 1.0);
        vec4 grassColor = vec4(0.0, 0.5, 0.0, 1.0);
        vec4 rockColor = vec4(0.3, 0.3, 0.3, 1.0);
        vec4 snowColor = vec4(0.5, 0.5, 0.5, 1.0);
        if (h < 0.5) {
            FragColor = waterColor;
        } 
        else if (h < 0.6) {
            FragColor = mix(waterColor, beachColor, (h - 0.5) / (0.6 - 0.5));
        } 
        else if (h < 0.75) {
            FragColor = mix(beachColor, grassColor, (h - 0.6) / (0.75 - 0.6));
        } 
        else if (h < 0.85) {
            FragColor = mix(grassColor, rockColor, (h - 0.75) / (0.85 - 0.75));
        } 
        else {
            FragColor = mix(rockColor, snowColor, (h - 0.85) / (1.0 - 0.85));
        }
    }
}
