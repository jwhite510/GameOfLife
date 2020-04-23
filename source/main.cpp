#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

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
  // count the number of neighbors
  for(int i=0; i < sim_grid.rows; i++) {
    for(int j=0; j < sim_grid.cols; j++) {

      // count the number of neighbors
      for(int _i=i-1; _i <=i+1; _i++) {
        for(int _j=j-1; _j <=j+1; _j++) {
          if(_i==i && _j==j)
            continue;
          if(_i >= 0 && _i<sim_grid.rows)
            if(_j >= 0 && _j<sim_grid.cols)
              countgrid(i,j) +=sim_grid(_i,_j);
        }
      }
    }
  }
  for(int i=0; i < sim_grid.rows; i++) {
    for(int j=0; j < sim_grid.cols; j++) {

      int cell_alive = sim_grid(i,j)==1;
      int two_or_three_neighbors = countgrid(i,j) == 2 || countgrid(i,j) == 3;
      int three_live_neighbors = countgrid(i,j) == 3;

      // any live cell with two or three live neighbors survives
      // ======================================================
      if(cell_alive && two_or_three_neighbors) // if cell is alive
      {
        countgrid(i,j) = 1; // cell lives
      }
      // any dead cell with three live neighbors becomes a live cell
      // ======================================================
      else if(!cell_alive && three_live_neighbors)
      {
        countgrid(i,j)=1;
      }
      // all other live cells die in the next generation
      // ======================================================
      else if(cell_alive)
      {
        countgrid(i,j)=0;
      }
      // all other dead cells stay dead
      else if(!cell_alive)
      {
        countgrid(i,j)=0;
      }
    }
  }
  for(int i=0; i < sim_grid.rows; i++)
    for(int j=0; j < sim_grid.cols; j++)
      sim_grid(i,j) = countgrid(i,j);
}
int main()
{

  cout << "game of life?" << endl;

  // number of pixels for with and height
  const int W = 1000;
  // number of grid points
  const int gridsize = 100;
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
      sim_grid(i,j) = 0; // initialize to 0

  // // blinker
  // sim_grid(4,4) = 1;
  // sim_grid(5,4) = 1;
  // sim_grid(3,4) = 1;

  // // beacon
  // sim_grid(10,10) = 1;
  // sim_grid(10,11) = 1;
  // sim_grid(11,11) = 1;
  // sim_grid(11,10) = 1;
  // // =====
  // sim_grid(12,12) = 1;
  // sim_grid(12,13) = 1;
  // sim_grid(13,13) = 1;
  // sim_grid(13,12) = 1;

  // populate the grid with random values
  srand(time(0));
  for(int i=0; i < 10000; i++)
    sim_grid(rand()%gridsize, rand()%gridsize) = 1;
  // add initial population to grid


  // populate corners
  // sim_grid(gridsize-1, gridsize-1) = 1;
  // sim_grid(0, gridsize-1) = 1;
  // sim_grid(gridsize-1, 0) = 1;
  // sim_grid(0, 0) = 1;

  auto draw_time = chrono::high_resolution_clock::now();
  int update_time_ms = 100;
  while(window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    // cout << time(0) << endl;
    // sf::sleep(sf::seconds(0.1));

    if(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-draw_time).count()>update_time_ms) {
      draw_time = chrono::high_resolution_clock::now();
      game_of_life_step(sim_grid, countgrid);
    }

    draw_grid(sim_grid, pixelgrid);
    texture.update(pixelgrid.pixels);
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
