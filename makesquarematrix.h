#include <vector>

inline std::vector<std::vector<float>> makeSquareMatrix(int r2)
{
    using std::vector;
    vector<vector<float>> res2(r2, vector<float>(r2));
    return res2;
}