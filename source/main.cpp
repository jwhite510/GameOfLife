#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

struct array2d
{
  int length;
  int rows;
  int cols;
  int* data;
  array2d(int rows, int cols)
  {
    this->rows = rows;
    this->cols = cols;
    length = rows*cols;
    data = new int[length];
  }
  inline int operator() (int get_row, int get_col) const
  {
    return data[get_row*cols + get_col];
  }
  inline int& operator() (int get_row, int get_col)
  {
    return data[get_row*cols + get_col];
  }
  ~array2d()
  {
    delete [] data;
  }
};

struct PixelGrid
{
  sf::Uint8* pixels;
  int W;
  int H;
  PixelGrid(int W_in, int H_in)
  {
    W = W_in;
    H = H_in;
    pixels = new sf::Uint8[W*H*4];
  }
  inline sf::Uint8 operator() (int row, int col, int color) const
  {
    return pixels[4*W*row + 4*col + color];
  }
  inline sf::Uint8& operator() (int row, int col, int color)
  {
    return pixels[4*W*row + 4*col + color];
  }
  ~PixelGrid()
  {
    cout << "PixelGrid destructor called" << endl;
    delete [] pixels;
  }
};

void draw_grid(array2d &sim_grid, PixelGrid &pixelgrid)
{
  // how many pixels are 1 simgrid
  int pixels_per_simgrid = pixelgrid.W / sim_grid.rows;
  // a square grid for now, only

  int pixelrow = 0;
  int pixelcol = 0;
  int grid_col_i = 0;
  int grid_row_i = 0;

  int i=0;
  for(int j=0; j < pixelgrid.W; j++) {
    pixelgrid(i,j,0) = 255;
    pixelgrid(i,j,1) = 255;
    pixelgrid(i,j,2) = 255;
    pixelgrid(i,j,3) = 255*sim_grid(0,grid_row_i);
    if(pixelrow > pixels_per_simgrid)
    {
      // increment grid index
      pixelrow = 0;
      std::cout << "pixels_per_simgrid" << " => " << pixels_per_simgrid << std::endl;
      std::cout << "grid_row_i" << " => " << grid_row_i << std::endl;
      grid_row_i++;
    }
    pixelrow++;

  }




  // pixelgrid.W
  // pixelgrid.H
  // sim_grid.rows
  // sim_grid.cols
}

int main()
{

  cout << "game of life?" << endl;

  // number of pixels for with and height
  const int W = 200;
  // number of grid points
  const int gridsize = 10;
  PixelGrid pixelgrid(W,W);
  sf::Texture texture;
  texture.create(W,W);
  sf::Sprite sprite(texture);
  sf::RenderWindow window(sf::VideoMode(W,W), "Game Of Life");

  // game gridspace
  array2d sim_grid(gridsize, gridsize);
  for(int i=0; i < gridsize; i++)
    for(int j=0; j < gridsize; j++)
      sim_grid(i,j) = 0;

  sim_grid(0,1) = 1;
  sim_grid(0,3) = 1;
  sim_grid(0,5) = 1;
  sim_grid(0,7) = 1;

  // print the sim_grid
  for(int i=0; i < gridsize; i++) {
    for(int j=0; j < gridsize; j++) {
      cout << sim_grid(i,j) << " ";
    }
    cout << endl;
  }

  // draw grid
  draw_grid(sim_grid, pixelgrid);


  while(window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    // for(int i=0; i < W; i++)
      // for(int j=0; j < W; j++) {
        // pixelgrid(i,j,0) = 255;
        // pixelgrid(i,j,1) = 0;
        // pixelgrid(i,j,2) = 0;
        // pixelgrid(i,j,3) = 255;
      // }
    texture.update(pixelgrid.pixels);
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
