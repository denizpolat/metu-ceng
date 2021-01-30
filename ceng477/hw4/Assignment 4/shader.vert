#version 330

layout(location = 0) in vec3 position;

// Data from CPU
uniform mat4 MVP; // ModelViewProjection Matrix

uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D heights;
uniform int widthTexture;
uniform int heightTexture;

// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


// finds the height = y component by checking x and z then returns the updated vertex;



//int isEmpty(vec3 x) {
//    if(x == (0,0,0)) return 1;
//    else return -1;
//}
//
//vec3 product(vec3 a, vec3 b) {
//    vec3 temp;
//    if( isEmpty(a) * isEmpty(b) == 1 ) {
//        return (0,0,0);
//    }
//    return (1,1,1);
//}








vec3 zero = vec3(0, 0, 0);




vec3 findHeight( vec3 vertex) {
    vec4 color = texture(heights, vec2(1 - vertex.x/(widthTexture + 1), 1 - vertex.z/(heightTexture +1))); // + 1 or not TODO
    vertex.y = color.x * heightFactor;
    return vertex;
}



vec3 calculateNormal(float a, float y, float b) {


    // a vertex has 6 neighbors

    // a __  b
    // |  \  |
    // d __  c

    vec3 center = vec3(position.x, findHeight(vec3(position.x, position.y, position.z)).y, position.z);


    float xplus1, xminus1, zplus1, zminus1;

    xplus1 = (a + 1 > widthTexture ? a : a + 1);
    xminus1 = (a -1 < 0 ? a : a - 1);
    zplus1 = (b + 1 > heightTexture ? b: b + 1);
    zminus1 = (b -1 < 0 ? b : b - 1);


    vec3 topLeft = findHeight(vec3(xplus1, 0, zminus1));
    vec3 top =     findHeight(vec3(xplus1,  0, b));
    vec3 right =   findHeight(vec3(a,       0, zplus1));
    vec3 botRight= findHeight(vec3(xminus1, 0, zplus1));
    vec3 bot =     findHeight(vec3(xminus1, 0, b   ));
    vec3 left =    findHeight(vec3(a,       0, zminus1));


    vec3 sum = vec3(0,0,0);
    vec3 edge1 = topLeft - center;
    vec3 edge2 = top - center;
    vec3 edge3 = right - center;
    vec3 edge4 = botRight - center;
    vec3 edge5 = bot - center;
    vec3 edge6 = left - center;

    sum += cross(edge2, edge1);
    sum += cross(edge3, edge2);
    sum += cross(edge4, edge3);
    sum += cross(edge5, edge4);
    sum += cross(edge6, edge5);
    sum += cross(edge1, edge6);
    return normalize(sum);







}

void main()
{
    vec4 color;
    vec4 position4 = vec4(position,1.);
    vec3 lightSource = vec3(widthTexture/2, 100, heightTexture/2);
    // get texture value, compute height

    textureCoordinate = vec2(1 - position.x/(widthTexture +1), 1 - position.z/(heightTexture +1)); // + 1 or not TODO
    color = texture(heights, textureCoordinate);


    position4.y = heightFactor * color.x;

    // compute normal vector using also the heights of neighbor vertices

    vertexNormal = calculateNormal(position.x, position.y, position.z);

    // compute toLight vector vertex coordinate in VCS

    ToLightVector = normalize(lightSource - vec3(MVP * position4));
    ToCameraVector = vec3(cameraPosition - position4);

    // set gl_Position variable correctly to give the transformed vertex position

    gl_Position = MVP * vec4(position4);


}
