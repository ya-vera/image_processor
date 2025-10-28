#include "filter.h"
#include <cmath>

bool FilterCrop::doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> /*floatParams*/)
{
  if (intParams.size() < 2) return false;
  if (intParams[0] < 0) return false;
  if (intParams[1] < 0) return false;
  int newWidth  = (intParams[0] > input.Width) ? input.Width : intParams[0];
  int newHeight = (intParams[1] > input.Height) ? input.Height : intParams[1];
  output.SetSize(newWidth, newHeight);
  for (int x=0;x<newWidth;x++)
      for (int y=0;y<newHeight;y++)
          output.Pixels[x][y] = input.Pixels[x][y];
  return true;
}

bool FilterGrayscale::doFilter(const Image &input, Image &output, std::vector<int> /*intParams*/, std::vector<float> /*floatParams*/)
{
  output.SetSize(input.Width, input.Height);
  for (int x=0;x<input.Width;x++)
      for (int y=0;y<input.Height;y++)
      {
          float Value = 0.299f * input.Pixels[x][y].Value[IDX_R] + 0.587f * input.Pixels[x][y].Value[IDX_G] + 0.114f * input.Pixels[x][y].Value[IDX_B];
          output.Pixels[x][y].Value[IDX_R] = Value;
          output.Pixels[x][y].Value[IDX_G] = Value;
          output.Pixels[x][y].Value[IDX_B] = Value;
          output.Pixels[x][y].saturate();
      }
  return true;
}

bool FilterNegative::doFilter(const Image &input, Image &output, std::vector<int> /*intParams*/, std::vector<float> /*floatParams*/)
{
  output.SetSize(input.Width, input.Height);
  for (int x=0;x<input.Width;x++)
      for (int y=0;y<input.Height;y++)
      {
          output.Pixels[x][y].Value[IDX_R] = input.Pixels[x][y].getMax() - input.Pixels[x][y].Value[IDX_R];
          output.Pixels[x][y].Value[IDX_G] = input.Pixels[x][y].getMax() - input.Pixels[x][y].Value[IDX_G];
          output.Pixels[x][y].Value[IDX_B] = input.Pixels[x][y].getMax() - input.Pixels[x][y].Value[IDX_B];
      }
  return true;
}


bool FilterMatrix::doFilter(const Image &input, Image &output, std::vector<int> intParams, std::vector<float> floatParams)
{
    initMatrix(intParams, floatParams);
    if (input.Width == 0) return false;
    if (input.Height == 0) return false;
    if (Matrix.size() == 0) return false;
    if (Matrix[0].size() == 0) return false;

    output.SetSize(input.Width, input.Height);
    for (int x=0;x<input.Width;x++)
      for (int y=0;y<input.Height;y++)
      {
          Pixel value;
          for (int dx = 0;dx < (int)Matrix.size();dx++)
              for (int dy=0;dy < (int)Matrix[dx].size();dy++)
              {
                  int sx = x + (dx - Matrix.size() / 2);
                  int sy = y + (dy - Matrix[dx].size() / 2);
                  if (sx < 0) sx = 0;
                  if (sx >= input.Width) sx = input.Width - 1;
                  if (sy < 0) sy = 0;
                  if (sy >= input.Height) sy = input.Height - 1;
                  value = value + (input.Pixels[sx][sy] * Matrix[dx][dy]);
              }
          value.saturate(); // For correct sharp-sharp result!
          output.Pixels[x][y] = value;
      }
  return true;
}


bool FilterSharpening::initMatrix(std::vector<int> /*intParams*/, std::vector<float> /*floatParams*/)
{
    Matrix.clear();
    std::vector<int> col;
    col.clear();    // Колонка 1
    col.push_back(0);
    col.push_back(-1);
    col.push_back(0);
    Matrix.push_back(col);
    col.clear();    // Колонка 2
    col.push_back(-1);
    col.push_back(5);
    col.push_back(-1);
    Matrix.push_back(col);
    col.clear();    // Колонка 3
    col.push_back(0);
    col.push_back(-1);
    col.push_back(0);
    Matrix.push_back(col);
    return true;
}

bool FilterEdge::initMatrix(std::vector<int> /*intParams*/, std::vector<float> /*floatParams*/)
{
    Matrix.clear();
    std::vector<int> col;
    col.clear();    // Колонка 1
    col.push_back(0);
    col.push_back(-1);
    col.push_back(0);
    Matrix.push_back(col);
    col.clear();    // Колонка 2
    col.push_back(-1);
    col.push_back(4);
    col.push_back(-1);
    Matrix.push_back(col);
    col.clear();    // Колонка 3
    col.push_back(0);
    col.push_back(-1);
    col.push_back(0);
    Matrix.push_back(col);
    return true;
}

bool FilterEdge::doFilter(const Image &input, Image &output, std::vector<int> /*intParams*/, std::vector<float> floatParams)
{
    Image temp;
    FilterGrayscale filter_g;
    if (floatParams.size() < 1) return false;
    if (!filter_g.doFilter(input, temp, std::vector<int>(), std::vector<float>())) return false;
    if (!FilterMatrix::doFilter(temp, output, std::vector<int>(), std::vector<float>())) return false;
    float threshold = floatParams[0] * Pixel::getMax();
    for (int x=0;x<input.Width;x++)
      for (int y=0;y<input.Height;y++)
      {
          if (output.Pixels[x][y].Value[IDX_R] < threshold)   // Grayscale - all fields are equal
          {
              output.Pixels[x][y].Value[IDX_R] = 0;
              output.Pixels[x][y].Value[IDX_G] = 0;
              output.Pixels[x][y].Value[IDX_B] = 0;
          }
          else
          {
              output.Pixels[x][y].Value[IDX_R] = Pixel::getMax();
              output.Pixels[x][y].Value[IDX_G] = Pixel::getMax();
              output.Pixels[x][y].Value[IDX_B] = Pixel::getMax();
          }
      }
    return true;
}

bool FilterGaussian::doFilter(const Image &input, Image &output, std::vector<int> /*intParams*/, std::vector<float> floatParams)
{
    if (floatParams.size() < 1) return false;

    // Инициализация ядра
    float sigma = floatParams[0];

    const size_t kernel_size = static_cast<size_t>(std::ceil(6*std::abs(sigma))) | 1;
    const size_t kernel_size_half = kernel_size / 2;
    std::vector<double> kernel(kernel_size);

    for (int dx = 0; dx <= static_cast<int>(kernel_size_half); ++dx)
    {
        kernel[kernel_size_half - dx] = 1 / sqrt(2 * 3.1415926535 * sigma * sigma) * exp(-dx*dx/(2*sigma*sigma));
        kernel[kernel_size_half + dx] = kernel[kernel_size_half - dx];
    }

    // Проход по X
    Image temp(input.Width, input.Height);
    for (int x=0;x<input.Width;x++)
       for (int y=0;y<input.Height;y++)
       {
          Pixel value;
          for (int dx = 0;dx < static_cast<int>(kernel_size);dx++)
          {
               int sx = x + (dx - kernel_size_half);
               if (sx < 0) sx = 0;
               if (sx >= input.Width) sx = input.Width - 1;
               value = value + (input.Pixels[sx][y] * kernel[dx]);
          }
          temp.Pixels[x][y] = value;
       }

    // Проход по Y
    output.SetSize(input.Width, input.Height);
    for (int x=0;x<input.Width;x++)
       for (int y=0;y<input.Height;y++)
       {
          Pixel value;
          for (int dx = 0;dx < static_cast<int>(kernel_size);dx++)
          {
               int sy = y + (dx - kernel_size_half);
               if (sy < 0) sy = 0;
               if (sy >= input.Height) sy = input.Height - 1;
               value = value + (temp.Pixels[x][sy] * kernel[dx]);
          }
          value.saturate();
          output.Pixels[x][y] = value;
       }

    return true;
}
