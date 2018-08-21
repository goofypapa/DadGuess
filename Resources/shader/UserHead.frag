varying vec2 v_texCoord;

void main(void)
{
    vec4 texColor = texture2D( CC_Texture0, v_texCoord );

    float distance = pow( ( v_texCoord.x - 0.5 ), 2.0 ) + pow( ( v_texCoord.y - 0.5 ), 2.0 );
    float radius = pow( 0.5, 2.0 );

    float alpha = texColor.a;
    if( distance > radius )
    {
        alpha = 0.0;
    }
    else if( radius - distance < 0.01 )
    {
        alpha = mix( 0.0, 1.0, (radius - distance) / 0.01 );
    }

    gl_FragColor = vec4( texColor.r, texColor.g, texColor.b, alpha );
}