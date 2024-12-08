#include <iostream>

using namespace std;

// Definición de un nodo de árbol AVL
template <typename T>
struct AVLNode {
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(T val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

// Clase de Árbol AVL
template <typename T>
class AVLTree {
private:
    AVLNode<T>* root;

    // Función auxiliar para obtener la altura de un nodo
    int height(AVLNode<T>* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // Función auxiliar para obtener el balance de un nodo
    int balanceFactor(AVLNode<T>* node) {
        return height(node->left) - height(node->right);
    }

    // Rotación a la derecha
    AVLNode<T>* rightRotate(AVLNode<T>* y) {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;

        // Realizar rotación
        x->right = y;
        y->left = T2;

        // Actualizar las alturas
        y->height = (height(y->left) > height(y->right) ? height(y->left) : height(y->right)) + 1;
        x->height = (height(x->left) > height(x->right) ? height(x->left) : height(x->right)) + 1;

        // Retornar nueva raíz
        return x;
    }

    // Rotación a la izquierda
    AVLNode<T>* leftRotate(AVLNode<T>* x) {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        // Realizar rotación
        y->left = x;
        x->right = T2;

        // Actualizar las alturas
        x->height = (height(x->left) > height(x->right) ? height(x->left) : height(x->right)) + 1;
        y->height = (height(y->left) > height(y->right) ? height(y->left) : height(y->right)) + 1;

        // Retornar nueva raíz
        return y;
    }

    // Función para insertar un nuevo nodo
    AVLNode<T>* insert(AVLNode<T>* node, T val) {
        // 1. Realizar inserción normal en el árbol binario de búsqueda
        if (node == nullptr) return new AVLNode<T>(val);

        if (val < node->data)
            node->left = insert(node->left, val);
        else if (val > node->data)
            node->right = insert(node->right, val);
        else
            return node; // No se permiten duplicados

        // 2. Actualizar la altura del nodo
        node->height = (height(node->left) > height(node->right) ? height(node->left) : height(node->right)) + 1;

        // 3. Verificar y balancear el árbol
        int balance = balanceFactor(node);

        // Si el nodo está desequilibrado, se realizan las rotaciones necesarias

        // Caso de rotación a la izquierda
        if (balance > 1 && val < node->left->data)
            return rightRotate(node);

        // Caso de rotación a la derecha
        if (balance < -1 && val > node->right->data)
            return leftRotate(node);

        // Caso de rotación izquierda-derecha
        if (balance > 1 && val > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Caso de rotación derecha-izquierda
        if (balance < -1 && val < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Función auxiliar para encontrar el nodo con el valor mínimo
    AVLNode<T>* minValueNode(AVLNode<T>* node) {
        AVLNode<T>* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Función para eliminar un nodo
    AVLNode<T>* deleteNode(AVLNode<T>* root, T val) {
        // Paso 1: Realizar la eliminación en el árbol binario de búsqueda
        if (root == nullptr) return root;

        if (val < root->data)
            root->left = deleteNode(root->left, val);
        else if (val > root->data)
            root->right = deleteNode(root->right, val);
        else {
            // Nodo con un solo hijo o sin hijos
            if (root->left == nullptr) {
                AVLNode<T>* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                AVLNode<T>* temp = root->left;
                delete root;
                return temp;
            }

            // Nodo con dos hijos: obtener el sucesor inorden
            AVLNode<T>* temp = minValueNode(root->right);

            // Copiar el sucesor inorden al nodo actual
            root->data = temp->data;

            // Eliminar el sucesor inorden
            root->right = deleteNode(root->right, temp->data);
        }

        // Paso 2: Actualizar la altura del nodo
        root->height = (height(root->left) > height(root->right) ? height(root->left) : height(root->right)) + 1;

        // Paso 3: Balancear el árbol
        int balance = balanceFactor(root);

        // Rotaciones si es necesario
        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Función auxiliar para realizar un recorrido en orden
    void inOrder(AVLNode<T>* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->data << " ";
            inOrder(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    // Función para insertar un valor en el árbol
    void insert(T val) {
        root = insert(root, val);
    }

    // Función para eliminar un nodo del árbol
    void deleteNode(T val) {
        root = deleteNode(root, val);
    }

    // Función para mostrar el recorrido en orden del árbol
    void inOrder() {
        inOrder(root);
        cout << endl;
    }
};

// Programa principal para mostrar el uso de la plantilla
int main() {
    AVLTree<int> tree;
    int cantidadNodos, valor, nodoEliminar;
    char respuesta;

    cout << "Ingrese la cantidad de nodos a insertar: ";
    cin >> cantidadNodos;

    // Ingresar los valores para los nodos
    for (int i = 0; i < cantidadNodos; i++) {
        cout << "Ingrese el valor para el nodo " << (i + 1) << ": ";
        cin >> valor;
        tree.insert(valor);
    }

    cout << "Arbol en orden despues de las inserciones: ";
    tree.inOrder();

    do {
        cout << "Ingrese el valor del nodo a eliminar: ";
        cin >> nodoEliminar;
        tree.deleteNode(nodoEliminar);

        cout << "Arbol en orden despues de la eliminacion: ";
        tree.inOrder();

        cout << "Desea eliminar otro nodo? (s/n): ";
        cin >> respuesta;

    } while (respuesta == 's' || respuesta == 'S');

    cout << "Programa terminado." << endl;
    return 0;
}
