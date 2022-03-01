#include <iostream>
#include <bits/stdc++.h>


using namespace std;


struct piece{
    int used;
    int position;
    int rot  = 0;
    /*
    int rot1[4];
    int rot2[4];
    int rot3[4];
    int rot4[4];
    */
    int rots[4][4];
}; 

void rotate(piece * p);
void solve_puzzle(piece * pool, piece * table, int i, int j, int num_columns);
int find_match(int fixed [4], int tmp [4]);
int is_match(piece t1, piece p2, bool esquerdacima);
void show_numbers(struct piece p);
void showlist(list<struct piece> g);

void rotate(piece * p){
    for(int i = 0; i < 4; i++){
        (*p).rots[1][i] = (*p).rots[0][(i+1)%4];
        (*p).rots[2][i] = (*p).rots[0][(i+2)%4];
        (*p).rots[3][i] = (*p).rots[0][(i+3)%4];
    }
    
}

int find_match(int fixed [4], int tmp [4]){
    
    for(int i = 0; i < 4; i++){
        for(int o = 0; o < 4; o++){
            if(fixed[i] == tmp[o+1] && fixed[i+1] == tmp[o] || fixed[i] == tmp[o-1] && fixed[i-1] == tmp[o]){
                
                return 1;
            }
        } 
    }
    return 0;
}




void solve_puzzle(vector<piece> pool, piece * table, int i, int j, int num_columns){ // nunca vamos tirar do saco a primeria peça porque ela já foi colocada no tabuleiro
    
    if((j+2)%4 == 1){ //coluna = 0
    
        if(is_match(table[j], pool[i], 1)){
            pool[i].position = j + 1;
            table[j+1] = pool[i];
            cout << "match0" << endl;
        }
    }
    else if((j+2)/4 == 0 ){ //linha = 0
        if(is_match(table[j], pool[i], 0)){
            
            pool[i].position = j + 1;
            table[j+1] = pool[i]; 
            cout << "match1" << endl;
        }
    }else{ // resto
        if(is_match(table[j], pool[i], 0) && is_match(table[j], pool[i], 1)){
            pool[i].position = j + 1;
            table[j+1] = pool[i]; 
            cout << "match2" << endl;
        }
    }
    cout << "macaca" << endl;

    //retirar da pool para uma pool de usados ou assim
    //dar return de forma recursiva                      
    
}


int is_match(piece t1, piece p2, bool esquerdacima){
    for(int i = 0; i < 4; i++){
        if(!esquerdacima){
            if(t1.rots[t1.rot][1] == p2.rots[i][0] && t1.rots[t1.rot][2] == p2.rots[i][3]){
                p2.rot = i;
                return 1;
            }
        }else{ 
            if (t1.rots[t1.rot][0] == p2.rots[i][3] && t1.rots[t1.rot][1] == p2.rots[i][2]){
                p2.rot = i;
                return 1;  
            }
        }
    }

    return 0;
}

void show_numbers(struct piece p){
    int i = 0;
    while(i< 4){
        cout << p.rots[0][i] << endl;
        i++;
    }

}

void showlist(list<struct piece> g)
{
    list<struct piece>::iterator it;
    for (it = g.begin(); it != g.end(); ++it){
        show_numbers(*it);
    }
    cout << '\n';
}


int main()
{
    
    int num_pieces;
    int num_lines;
    int num_columns, num_puzzles;

    cin >> num_puzzles;
    do{
        cout << "Insira variaveis validas:" << endl;
        cin >> num_pieces >> num_lines >> num_columns;
        cout << "numpieces: " << num_pieces << endl;
        cout << "numlines: " << num_lines << endl;
        cout << "numcolumns: " << num_columns << endl;
    }while(num_lines * num_columns != num_pieces);

    vector<piece> pool;
    piece table [num_pieces];
    
    struct piece  p;
    for(int u = 0; u < num_pieces; u++){
        for(int i = 0; i < 4; i++)
        {
	        cin >> p.rots[0][i];
        }
        rotate(&p);
        if(u == 0)
            table[0] = p;
        else
            pool.push_back(p);
    }
    /*
    for(int u = 0; u < num_pieces; u++){
        for(int i = 0; i < 4; i++)
        {
            cout << pool[u].rots[0][i] ;
        }
        cout << endl;
    }*/


    solve_puzzle(pool, table, 0, 0, num_columns);

    //por primeira peca na primeira posicao do tabuleiro
    /*
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot2[i] ;
    }
        cout << endl;
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot3[i] ;
    }
        cout << endl;
    for(int i = 0; i < 4; i++)
    {
        cout << p.rot4[i] ;
    }
    cout << endl;
    */

    //lista.push_front(p);

    
    //int c = find_match((*lista.begin()).numbers,(*std::next(lista.begin(), 1)).numbers);
    //cout << "olha a match:" <<c << endl;
    //showlist(lista);

    return 0;
}