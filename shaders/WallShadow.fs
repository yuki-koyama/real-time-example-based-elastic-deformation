void main()
{
	float depth = gl_FragCoord.z;
	depth = 1.0 - depth;
	depth = depth * 30.0;

	gl_FragDepth = gl_FragCoord.z;

	gl_FragColor = vec4(depth, depth, depth, 1);
}
