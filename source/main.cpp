#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

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

int main()
{

  cout << "game of life?" << endl;
  int W = 200;
  int H = 200;
  PixelGrid pixelgrid(W,H);
  sf::Texture texture;
  texture.create(W,H);
  sf::Sprite sprite(texture);
  sf::RenderWindow window(sf::VideoMode(W,H), "Game Of Life");
  while(window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    for(int i=0; i < W; i++)
      for(int j=0; j < H; j++) {
        pixelgrid(i,j,0) = 255;
        pixelgrid(i,j,1) = 0;
        pixelgrid(i,j,2) = 0;
        pixelgrid(i,j,3) = 255;
      }
    texture.update(pixelgrid.pixels);
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
