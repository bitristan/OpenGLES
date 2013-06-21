uniform mat4 uOrientationM;
uniform mat4 uTransformM;
attribute vec2 position;

varying vec2 vTextureCoord;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	vTextureCoord = (uTransformM * ((uOrientationM * gl_Position + 1.0) * 0.5)).xy;
}