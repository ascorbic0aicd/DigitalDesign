int seed = 0;
void Qsrand(int s)
{
    seed = s;
} 
unsigned Qrand()
{
    seed = (seed * 1103515245 + 12345) % 2147483648; // LCG算法公式
    return (int)seed;

}