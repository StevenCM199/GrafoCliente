#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#define PORT 8080
#define V 9

using namespace std;

// ----- VARIABLES GLOBALES ----- //

int sock = 0;
struct sockaddr_in serv_addr;
int opcion = 0;

/*!
 * \brief
 * Funcion que crea el socket del cliente y busca conectarlo al servidor
 * si la conexion es exitosa, la funcion despliega una interfaz sobre la que el usuario puede ordenarle
 * al servidor que realice los caluclos sobre el grafo
 * @return
 */

int CrearSocket(){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    cout << "Conexion con el servidor exitosa" << endl;

    bool exit = true;
    while(exit){
        cout << "-----------------------------------" << endl;
        cout << "/// Introduce una opcion /// " << endl;
        cout << "1. Crear el grafo" << endl;
        cout << "2. Solicitar la ruta mas corta del grafo al servidor" << endl;
        cout << "3. Desconectar" <<endl;
        cin >> opcion;
        switch(opcion){
            case 1:
                cout << ("Se creara el grafo") << endl;
                send (sock, &opcion, sizeof(int), 0 );

                int arr[V][V];
                recv(sock, &arr, sizeof(int) * 9 * 9, 0);

                int FILA, COLUMNA;
                printf("Los elementos del grafo son: \n");
                for (FILA = 0; FILA < V; FILA++) {
                    for (COLUMNA = 0; COLUMNA < V; COLUMNA++) {
                        printf("%d " , arr[FILA][COLUMNA]);
                    }
                    printf("\n");
                }
                break;

            case 2:
                cout << ("Enviando solicitud...") << endl;
                send (sock, &opcion, sizeof(int), 0 );

                int path[V];
                recv(sock, &path, sizeof(int) * V, 0);

                if (path[1] == 0){
                    cout << "El cliente aun no ha calculado el grafo" << endl;
                    break;
                }

                cout << "Las distancias han sido calculadas:" << endl;
                cout << "Vertice" << " \t \t " << "Distancia desde nodo 0" << endl;

                for (int i = 0; i < V; i++){
                    cout << i << "\t \t \t \t \t" << path[i] << endl;
                }
                break;

            case 3:
                cout << "Desconectando...";
                send (sock, &opcion, sizeof(int), 0 );
                exit = false;
                break;

            default:
                send (sock, &opcion, sizeof(int), 0 );
                break;
        }
    }
    return 0;
}

/*!
 * \brief
 * funcion encargada de esperar al servidor para realizar la conexionn
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char const *argv[])
{
    cout<< "Esperando al servidor..." << endl;
    CrearSocket();


    return 0;
}