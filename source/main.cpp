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
  // for each row
  for(int i=0; i < pixelgrid.W; i++) {
    grid_col_i = 0;
    pixelcol = 0;
    for(int j=0; j < pixelgrid.W; j++) {
      pixelgrid(i,j,0) = 0;
      pixelgrid(i,j,1) = 0;
      pixelgrid(i,j,2) = 255;
      pixelgrid(i,j,3) = 255*sim_grid(grid_row_i,grid_col_i);
      // pixelgrid(i,j,3) = value;
      if(pixelcol >= pixels_per_simgrid) {
        // increment grid index
        pixelcol = 0;
        grid_col_i++;
      }
      pixelcol++;
    }
    if(pixelrow >= pixels_per_simgrid) {
      pixelrow = 0;
      grid_row_i++;
    }
    pixelrow++;

  }




  // pixelgrid.W
  // pixelgrid.H
  // sim_grid.rows
  // sim_grid.cols
}
void game_of_life_step(array2d &sim_grid, array2d &countgrid)
{
  // zero the count grid
  for(int i=0; i < countgrid.rows; i++)
    for(int j=0; j < countgrid.cols; j++)
      countgrid(i,j) = 0;


  // count neighbors of each cell
  cout << "++++++++++++++++++++" << endl;
  cout << "count neighbors:" << endl;
  for(int i=0; i < sim_grid.rows; i++) {
    for(int j=0; j < sim_grid.cols; j++) {
      cout << sim_grid(i,j) << "  ";
    }
    cout << endl;
  }

  // count the number of neighbors
  for(int i=0; i < sim_grid.rows; i++) {
    for(int j=0; j < sim_grid.cols; j++) {

      // count the rows on left and right
      if(j==0 || j==sim_grid.cols-1) {
        if(i!=0 && i!=sim_grid.rows-1)
          countgrid(i,j) = 99;
      }

      // count the rows on top and bottom
      if(i==0 || i==sim_grid.rows-1) {
        if(j!=0 && j!=sim_grid.cols-1)
          countgrid(i,j) = 98;
      }

      // count the corners
      if(i==0 && j==0) {
        countgrid(i,j) = 50;
      }
      if(i==sim_grid.rows-1 && j==sim_grid.cols-1) {
        countgrid(i,j) = 51;
      }
      if(i==sim_grid.rows-1 && j==0) {
        countgrid(i,j) = 52;
      }
      if(i==0 && j==sim_grid.cols-1) {
        countgrid(i,j) = 53;
      }



    }
  }

  cout << "--------------------" << endl;


  // count
  for(int i=0; i < countgrid.rows; i++) {
    for(int j=0; j < countgrid.cols; j++) {
      cout << countgrid(i,j) << "  ";
    }
    cout << endl;
  }


}
int main()
{

  cout << "game of life?" << endl;

  // number of pixels for with and height
  const int W = 600;
  // number of grid points
  const int gridsize = 5;
  PixelGrid pixelgrid(W,W);
  sf::Texture texture;
  texture.create(W,W);
  sf::Sprite sprite(texture);
  sf::RenderWindow window(sf::VideoMode(W,W), "Game Of Life");

  // game gridspace
  array2d sim_grid(gridsize, gridsize);
  array2d countgrid(gridsize, gridsize);
  for(int i=0; i < gridsize; i++)
    for(int j=0; j < gridsize; j++)
      sim_grid(i,j) = 0;


    // populate the grid with random values
  srand(time(0));
  for(int i=0; i < 10; i++)
    sim_grid(rand()%gridsize, rand()%gridsize) = 1;
  // add initial population to grid

  while(window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    sf::sleep(sf::seconds(1));
    cout << "sleeo" << endl;
    // for(int i=0; i < 10; i++)
      // sim_grid(rand()%gridsize, rand()%gridsize) = rand()%2;
    game_of_life_step(sim_grid, countgrid);

    // sim_grid(9,9) = 1;
    // sim_grid(0,9) = 1;
    // sim_grid(0,0) = 1;
    // sim_grid(3,2) = 1;
    // sim_grid(3,4) = 1;
    // sim_grid(3,6) = 1;
    // sim_grid(9,0) = 1;
    // sim_grid(9,0) = 1;


    draw_grid(sim_grid, pixelgrid);


    texture.update(pixelgrid.pixels);
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
