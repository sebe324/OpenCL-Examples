__kernel void ProcessTwoDimArray(__global int* inData)
{
int row=get_global_id(1);
int col=get_global_id(0);
int cols=get_global_size(0);

size_t id=(row*cols)+col;

inData[id]=inData[id]*2;
}