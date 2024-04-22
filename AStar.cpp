#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#define ROWS 20
#define COLS 20
#define STARTX 19 
#define STARTY 3 
#define GOALX 8
#define GOALY 19

using namespace std;

class Node // Klasa definiująca węzeł (pole) na siatce
{
public:
    int x, y; // Współrzędne określające położenie węzła na mapie, X - oś góra/dół, Y - oś lewo/prawo
    Node* parent; //Adres węzła rodzica, czyli poprzedniego węzła na ścieżce
    double g, h; // g = koszt od startu do danego węzła, h = wartość heurystyczna od startu do danego węzła
    Node(int x, int y, Node* parent, double g, double h):
    x(x), y(y), parent(parent), g(g), h(h) {}

    double f() const //wartość węzła, im mniejsza tym dany węzeł będzie pierwszy do rozważenia
    {
        return g + h;
    } 
};

class Map // Klasa definiująca mapę (grida), czyli siatkę pól kwadratów na którym będziemy używać algorytmu
{
public:
    int dane[ROWS][COLS];
    Map(const char* filename) // wczytanie mapy na wyjście
    {
        ifstream file(filename);
        if(!file)
        {
            cout << "Nie można otworzyć pliku "<< filename <<endl;
            exit(1);
        }
        for (unsigned int i = 0; i<ROWS; ++i)
            for (unsigned int j = 0; j<COLS; ++j)
                file >> dane[i][j];
    }
    
    double euclidean_heuristics(int x, int y) // metoda obliczająca wartość heurystyczną metodą odległości Euklidesowej
    {
        return sqrt(pow(x-GOALX, 2) + pow(y-GOALY, 2));
    }
       
    bool isGoal(int x, int y) // metoda sprawdzająca czy dany węzeł jest celem poszukiwań
    {
        return x == GOALX && y == GOALY;
    }   

    bool isEmptySpace(int x, int y) // metoda sprawdzająca czy dany węzeł nie jest przeszkodą lub nie wychodzimy poza obszar mapy
    {
        return x >= 0 && y >= 0 && x < ROWS && y < COLS && dane[x][y] == 0;
    }
        
    void setRoute(Node* Node) // procedura wyznaczająca trasę na mapie trójkami
    {
        while (Node)
        {
            dane[Node->x][Node->y] = 3;
            Node = Node->parent;
        }
    }
};

bool isInList(list<Node*>& lista, int x, int y) // funkcja sprawdzająca czy dany węzeł jest na podanej liście
{
    for(Node* Node : lista)
    {
        if (Node->x == x && Node->y == y)
            return true;
    }
    return false;
}

void AStar(Map& grid)
{
    list<Node*> OpenList;
    list<Node*> ClosedList;

    Node* start = new Node(STARTX, STARTY, nullptr, 0, grid.euclidean_heuristics(STARTX, STARTY));
    OpenList.push_back(start);

    while (!OpenList.empty()) {
        Node* curr = OpenList.front();
        for (Node* Node : OpenList) {
            if (Node->f() < curr->f() || (Node->f() == curr->f() && rand() % 2 == 0)) 
                curr = Node;    
        }

        OpenList.remove(curr);
        ClosedList.push_back(curr);

        if(grid.isGoal(curr->x, curr->y))
        {
            grid.setRoute(curr);
            break;
        }

        int movementX[] = {0, 0, -1, 1};
        int movementY[] = {-1, 1, 0, 0};

        for(int i = 0; i < 4; ++i)
        {
            int x = curr->x + movementX[i];
            int y = curr->y + movementY[i];

            if(!grid.isEmptySpace(x, y) || isInList(ClosedList, x, y))
                continue;

            double g = curr->g + 1;
            double h = grid.euclidean_heuristics(x, y);
            Node* neighbour = new Node(x, y, curr, g, h);

            if (!isInList(OpenList, x, y) || g < neighbour->g) 
                OpenList.push_back(neighbour);
        }   
    }

    if(OpenList.empty())
        cout<<"Brak mozliwosci dojscia do celu\n"<<endl;

    for(Node* Node: OpenList)
    {
        delete Node;
    }

    for(Node* Node: ClosedList)
    {
        delete Node;
    }
}

int main()
{
    string filename = "grid.txt";
    cout << "Nacisnij ENTER aby wczytac mape z pliku " <<filename<<endl;
    getchar();
    Map grid(filename.c_str());
    if(grid.dane[STARTX][STARTY] == 5)
    {
      cout<<"Punkt startowy jest przeszkoda\n"<<endl;
      for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << grid.dane[i][j] << " ";
        }
        cout << endl; 
        }
        cout<<"\nNacisnij ENTER aby zakonczyc";
        getchar();
        return 0; 
    }
    AStar(grid);
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << grid.dane[i][j] << " ";
        }
        cout << endl;
    }
    cout<<"\nNacisnij ENTER aby zakonczyc";
    getchar();
    return 0;
}
