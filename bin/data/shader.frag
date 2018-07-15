#version 150

uniform sampler2DRect image;
uniform sampler2DRect mask;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec4 texel0 = texture(image, texCoordVarying);
    float alpha = texture(mask, texCoordVarying); // single channel, and thus single float value. 
    //outputColor = vec4(texel0.rgb, texel0.a * texel1.a); // why the multiplication?
    outputColor = vec4(texel0.rgb, alpha);
}