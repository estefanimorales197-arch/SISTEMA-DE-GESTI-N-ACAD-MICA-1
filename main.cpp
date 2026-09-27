#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>

using namespace std;

struct Student
{
    string id;
    string name;
};

struct Course
{
    string id;
    string description;
};

struct Calification
{
    string idCourse;
    string idStudent;
    float value;
};

struct Group
{
    string id;
    string idCourse;
    vector<string> studentIDs;
};

// Definición de colas y pilas
// –––––––––––––––––––––––––––

struct AdvisingRequest
{
    int id;
    string idStudent;
    string idCourse;
    string reason;
    bool isAttended;
};

struct RequestNode
{
    AdvisingRequest data;
    RequestNode *nextRequestNode;
};

struct RequestList
{
    RequestNode *head = nullptr;
    RequestNode *tail = nullptr;
    int size = 0;
};

struct ReferenceNode
{
    AdvisingRequest *request;
    ReferenceNode *nextReferenceNode;
};

struct RequestQueue
{
    ReferenceNode *front = nullptr;
    ReferenceNode *rear = nullptr;
    int size = 0;
};

struct RequestStack
{
    ReferenceNode *top = nullptr;
    int size = 0;
};

struct Database
{
    vector<Student> students;
    vector<Course> courses;
    vector<Calification> califications;
    vector<Group> groups;

    RequestList requests;
    RequestQueue pending;
    RequestStack history;
    int nextRequestNumber = 1;
};

/**
 * @brief Muestra el menú de opciones
 * @return Devuelve el número de la opción que elige el usuario.
 */
int showMenu()
{
    int option = 0;
    cout << "=================================================" << endl;
    cout << "Libro académico: " << endl;
    cout << "=================================================" << endl;
    cout << "1. Registrar curso." << endl;
    cout << "2. Registrar grupo." << endl;
    cout << "3. Registrar estudiante." << endl;
    cout << "4. Matricular estudiante." << endl;
    cout << "5. Asignar calificación." << endl;
    cout << "6. Ver grupo." << endl;
    cout << "7. Ver todo." << endl;
    cout << "---- Atención / asesoría académica ----\n";
    cout << "8. Solicitar atención/asesoría académica\n";
    cout << "9. Atender siguiente solicitud (cola)\n";
    cout << "10. Ver cola de solicitudes pendientes\n";
    cout << "11. Ver historial de entrada (pila)\n";
    cout << "12. Ver lista de todas las solicitudes\n";
    cout << "0. Salir" << endl;
    cout << "=================================================" << endl;
    if (!(cin >> option))
    {
        if (cin.eof())
        {
            return 0;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }

    return option;
}

/**
 * @brief Busca y retorna, si existe, un curso.
 * @param database Base de datos donde se hará la búsqueda.
 * @param id Identificador del curso.
 * @return Curso.
 */
Course *findCourseById(Database &database, const string &id)
{
    for (auto &course : database.courses)
    {
        if (course.id == id)
        {
            return &course;
        }
    }

    return nullptr;
}

/**
 * @brief Solicita el código y la descripción de un curso; si el código no existe, registra el curso.
 * @param database Base de datos donde se almacenará el curso.
 */
void addCourse(Database &database)
{
    string id, description;
    cout << "Ingrese el código del curso: ";
    cin >> id;

    if (findCourseById(database, id))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Ya existe un curso con el mismo código\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingresa la descripción del curso: ";
    getline(cin, description);

    database.courses.push_back({id, description});

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Curso registrado con éxito.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Busca y devuelve, en caso de existir, un grupo.
 * @param database Base de datos dónde hará la búsqueda.
 * @param id Identificador único del grupo a buscar.
 * @return Grupo.
 */

Group *findGroupById(Database &database, const string &id)
{
    for (auto &group : database.groups)
    {
        if (group.id == id)
        {
            return &group;
        }
    }
    return nullptr;
}

/**
 * @brief Se le pedirá al usuario la información del grupo y el curso; validará existencias: si el grupo no existe, valida existencia del curso: si no existe, crea el grupo. De lo contrario, deja un aviso y regresa al menú.
 * @param database La base de datos donde se agregará el grupo
 * @return Guarda en la base de datos, el grupo con su respectivo curso creado.
 */

void addGroup(Database &database)
{

    string id, idCourse;
    cout << "Ingrese el código del grupo: ";
    cin >> id;

    if (findGroupById(database, id))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Ya existe un grupo con el mismo código\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingresa el código del curso: ";
    getline(cin, idCourse);

    if (!findCourseById(database, idCourse))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El curso no existe. Créalo e inténtalo de nuevo.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    database.groups.push_back({id, idCourse, {}});

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Grupo registrado con éxito.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Busca y retorna, si existe, un estudiante.
 * @param database Base de datos en la que se buscará.
 * @param id Identificador único del estudiante que se va a buscar.
 * @return Puntero al estudiante, o nullptr si no existe.
 */
Student *findStudentById(Database &database, const string &id)
{
    for (auto &student : database.students)
    {
        if (student.id == id)
        {
            return &student;
        }
    }
    return nullptr;
}

/**
 * @brief Consulta si existe el estudiante a registrar; si no existe, lo agrega.
 * @param Database Base de datos donde se almacenará el estudiante.
 * @return Guarda el estudiante.
 */
void addStudent(Database &database)
{
    string id, name;
    cout << "Ingresa la identificación del estudiante: ";
    cin >> id;

    if (findStudentById(database, id))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El estudiante ya existe.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingresa el nombre del estudiante: ";
    getline(cin, name);

    database.students.push_back({id, name});

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Estudiante registrado con éxito.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Consulta si dicho estudiante está matriculado en dicho curso.
 * @param database Base de datos a la que consultará
 * @param idStudent Identificador único del estudiante
 * @param idCourse Identificador único del curso
 * @return Si existe o no
 */
bool isEnrolledStudentInCourse(Database &database, const string &idStudent, const string &idCourse)
{
    for (const auto &group : database.groups)
    {
        if (group.idCourse != idCourse)
        {
            continue;
        }

        for (const auto &id : group.studentIDs)
        {
            if (id == idStudent)
            {
                return true;
            }
        }
    }
    return false;
}

Calification *findCalification(Database &database, const string &idStudent, const string &idCourse)
{
    for (auto &calification : database.califications)
    {
        if (calification.idStudent == idStudent && calification.idCourse == idCourse)
        {
            return &calification;
        }
    }
    return nullptr;
}

/**
 * @brief Valida existencia del estudiante, del curso y de la matrícula para asignar la nota.
 * @param Database Base de datos en la que consultará la existencia de cada entidad y se registrará la calificación.
 * @return Asigna la respectiva calificación del estudiante en el curso matriculado.
 */
void addCalification(Database &database)
{
    string idStudent;
    cout << "Ingrese la identificación del estudiante: ";
    cin >> idStudent;

    if (!findStudentById(database, idStudent))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "Estudiante no encontrado.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    string idCourse;
    cout << "Ingrese el código del curso: ";
    cin >> idCourse;

    if (!findCourseById(database, idCourse))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "Curso no encontrado.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    if (!isEnrolledStudentInCourse(database, idStudent, idCourse))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "El estudiante no está matriculado en ese curso.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    float value;
    cout << "Ingrese la calificación (0.0 - 5.0): ";
    if (!(cin >> value) || value < 0.0f || value > 5.0f)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "La calificación debe ser un número entre 0.0 y 5.0.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    /**
     * @note Si ya tenía nota, la actualiza.
     */
    if (Calification *existing = findCalification(database, idStudent, idCourse))
    {
        existing->value = value;
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "Calificación actualizada exitosamente.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    database.califications.push_back({idCourse, idStudent, value});

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Calificación registrada exitosamente.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Matricula un estudiante luego de validar la existencia del mismo, la existencia del grupo y que no se encuentre ya matriculado al curso.
 * @return Registra o no al estudiante al respectivo curso matriculado.
 */
void enrollStudent(Database &database)
{
    string idGroup, idStudent;
    cout << "Ingresa el grupo al que desea matricular: ";
    cin >> idGroup;

    Group *group = findGroupById(database, idGroup);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!group)
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El grupo ingresado no existe.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cout << "Ingresa el idendificador del estudiante: ";
    cin >> idStudent;

    if (!findStudentById(database, idStudent))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El estudiante ingresado no existe.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    if (isEnrolledStudentInCourse(database, idStudent, group->idCourse))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El estudiante ya se encuentra matriculado en el curso.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    group->studentIDs.push_back(idStudent);

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Estudiante matriculado con éxito.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Muestra toda la información registrada en la base de datos.
 */
void listAll(const Database &database)
{
    cout << "\nEstudiantes (" << database.students.size() << "):\n";
    for (const auto &student : database.students)
    {
        cout << "  " << student.id << " - " << student.name << "\n";
    }
    cout << "Cursos (" << database.courses.size() << "):\n";
    for (const auto &course : database.courses)
    {
        cout << "  " << course.id << " - " << course.description << "\n";
    }
    cout << "Grupos (" << database.groups.size() << "):\n";
    for (const auto &group : database.groups)
    {
        cout << "  " << group.id << " (curso " << group.idCourse << ", "
             << group.studentIDs.size() << " estudiantes)\n";
    }
    cout << endl;
}

void showGroup(Database &database)
{
    string idGroup;
    cout << "Ingrese el código del grupo: ";
    cin >> idGroup;

    const Group *group = findGroupById(database, idGroup);
    if (!group)
    {
        cout << "Grupo no encontrado.\n";
        return;
    }
    const Course *course = findCourseById(database, group->idCourse);

    cout << "\n====================================\n";
    cout << "Grupo: " << group->id << "\n";
    cout << "Curso: " << course->id << " - " << course->description << "\n";
    cout << "====================================\n";

    if (group->studentIDs.empty())
    {
        cout << "El grupo no tiene estudiantes matriculados.\n\n";
        return;
    }

    float sum = 0.0f;
    int calificationCount = 0;

    for (const auto &studentId : group->studentIDs)
    {
        const Student *student = findStudentById(database, studentId);
        const Calification *calification = findCalification(database, studentId, group->idCourse);

        cout << student->id << " - " << student->name << ": ";
        if (calification)
        {
            cout << calification->value << "\n";
            sum += calification->value;
            calificationCount++;
        }
        else
        {
            cout << "Sin nota\n";
        }
    }

    cout << "------------------------------------\n";
    cout << "Estudiantes: " << group->studentIDs.size() << "\n";
    if (calificationCount > 0)
    {
        cout << "Promedio del grupo: " << sum / calificationCount << "\n";
    }
    cout << defaultfloat << endl;
}

void seedDatabase(Database &database)
{
    database.students.push_back({"1234567", "JOHAN ALEXANDER"});
    database.courses.push_back({"BD01", "BASES DE DATOS I"});
    database.groups.push_back({"G100", "BD01", {"1234567"}});
    database.califications.push_back({"BD01", "1234567", 4.5f});
}

/**
 * @brief Agrega una solicitud al final de la lista enlazada de solicitudes.
 * @param list Lista donde se almacenará la solicitud.
 * @param request Solicitud que se agregará.
 * @return Puntero a la solicitud almacenada en la lista.
 */
AdvisingRequest *appendRequest(RequestList &list, const AdvisingRequest &request)
{
    RequestNode *node = new RequestNode{request, nullptr};

    if (list.tail)
    {
        list.tail->nextRequestNode = node;
    }
    else
    {
        list.head = node;
    }
    list.tail = node;
    list.size++;

    return &node->data;
}

/**
 * @brief Encola la referencia a una solicitud al final de la cola de pendientes.
 * @param queue Cola de solicitudes pendientes.
 * @param request Solicitud que se encolará.
 */
void enqueueRequest(RequestQueue &queue, AdvisingRequest *request)
{
    ReferenceNode *node = new ReferenceNode{request, nullptr};

    if (queue.rear)
    {
        queue.rear->nextReferenceNode = node;
    }
    else
    {
        queue.front = node;
    }
    queue.rear = node;
    queue.size++;
}

/**
 * @brief Retira la solicitud que está al frente de la cola de pendientes.
 * @param queue Cola de solicitudes pendientes.
 * @return Puntero a la solicitud retirada, o nullptr si la cola está vacía.
 */
AdvisingRequest *dequeueRequest(RequestQueue &queue)
{
    if (!queue.front)
    {
        return nullptr;
    }

    ReferenceNode *node = queue.front;
    AdvisingRequest *request = node->request;

    queue.front = node->nextReferenceNode;
    if (!queue.front)
    {
        queue.rear = nullptr;
    }
    queue.size--;
    delete node;

    return request;
}

/**
 * @brief Apila la referencia a una solicitud en el historial de entrada.
 * @param stack Pila del historial de entrada.
 * @param request Solicitud que se apilará.
 */
void pushRequest(RequestStack &stack, AdvisingRequest *request)
{
    stack.top = new ReferenceNode{request, stack.top};
    stack.size++;
}

/**
 * @brief Muestra los datos de una solicitud de asesoría.
 * @param request Solicitud que se mostrará.
 */
void printRequest(const AdvisingRequest &request)
{
    cout << "  #" << request.id
         << " | Estudiante: " << request.idStudent
         << " | Curso: " << request.idCourse
         << " | Estado: " << (request.isAttended ? "Atendida" : "Pendiente")
         << "\n    Motivo: " << request.reason << "\n";
}

/**
 * @brief Registra una solicitud de asesoría luego de validar la existencia del estudiante y del curso; la agrega a la lista, la encola como pendiente y la apila en el historial.
 * @param database Base de datos donde se registrará la solicitud.
 */
void requestAdvising(Database &database)
{
    string idStudent, idCourse, reason;

    cout << "Ingresa el identificador del estudiante: ";
    cin >> idStudent;

    if (!findStudentById(database, idStudent))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "El estudiante ingresado no existe.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cout << "Ingrese el código del curso: ";
    cin >> idCourse;

    if (!findCourseById(database, idCourse))
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "Curso no encontrado.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingresa el motivo de la solicitud de asesoría: ";
    getline(cin, reason);

    AdvisingRequest *request = appendRequest(
        database.requests, {database.nextRequestNumber++, idStudent, idCourse, reason, false});
    enqueueRequest(database.pending, request);
    pushRequest(database.history, request);

    cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "Solicitud #" << request->id << " registrada con éxito.\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

/**
 * @brief Atiende la siguiente solicitud pendiente de la cola y la marca como atendida.
 * @param database Base de datos que contiene la cola de pendientes.
 */
void attendNext(Database &database)
{
    AdvisingRequest *request = dequeueRequest(database.pending);

    if (!request)
    {
        cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "No hay solicitudes pendientes.\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
        return;
    }

    request->isAttended = true;

    cout << "\nSolicitud atendida:\n";
    printRequest(*request);
    cout << "Quedan " << database.pending.size << " solicitudes pendientes.\n\n";
}

/**
 * @brief Muestra las solicitudes pendientes en orden de atención, del frente al final de la cola.
 * @param database Base de datos que contiene la cola de pendientes.
 */
void showPendingQueue(const Database &database)
{
    cout << "\nSolicitudes pendientes (" << database.pending.size << "):\n";
    for (ReferenceNode *node = database.pending.front; node; node = node->nextReferenceNode)
    {
        printRequest(*node->request);
    }
    cout << endl;
}

/**
 * @brief Muestra el historial de entrada desde la solicitud más reciente hasta la más antigua.
 * @param database Base de datos que contiene la pila del historial.
 */
void showEntryHistory(const Database &database)
{
    cout << "\nHistorial de entrada (" << database.history.size << "):\n";
    for (ReferenceNode *node = database.history.top; node; node = node->nextReferenceNode)
    {
        printRequest(*node->request);
    }
    cout << endl;
}

/**
 * @brief Muestra todas las solicitudes registradas en el orden en que fueron creadas.
 * @param database Base de datos que contiene la lista de solicitudes.
 */
void showAllRequests(const Database &database)
{
    cout << "\nTodas las solicitudes (" << database.requests.size << "):\n";
    for (RequestNode *node = database.requests.head; node; node = node->nextRequestNode)
    {
        printRequest(node->data);
    }
    cout << endl;
}

/**
 * @brief Libera la memoria de la cola, la pila y la lista de solicitudes.
 * @param database Base de datos cuyas solicitudes se liberarán.
 */
void clearRequests(Database &database)
{
    while (dequeueRequest(database.pending))
    {
    }

    while (database.history.top)
    {
        ReferenceNode *node = database.history.top;
        database.history.top = node->nextReferenceNode;
        delete node;
    }
    database.history.size = 0;

    while (database.requests.head)
    {
        RequestNode *node = database.requests.head;
        database.requests.head = node->nextRequestNode;
        delete node;
    }
    database.requests.tail = nullptr;
    database.requests.size = 0;
}

int main()
{
    Database database;
    seedDatabase(database);

    while (true)
    {
        switch (showMenu())
        {
        case 1:
            cout << "Registrar curso" << endl;
            cout << "=================================================" << endl;
            addCourse(database);
            break;

        case 2:
            cout << "Registrar grupo" << endl;
            cout << "=================================================" << endl;
            addGroup(database);
            break;

        case 3:
            cout << "Registrar estudiante" << endl;
            cout << "=================================================" << endl;
            addStudent(database);
            break;
        case 4:
            cout << "Matricular estudiante" << endl;
            cout << "=================================================" << endl;

            enrollStudent(database);

            break;
        case 5:
            cout << "Asignar calificación" << endl;
            cout << "=================================================" << endl;
            addCalification(database);
            break;
        case 6:
            cout << "Ver grupo" << endl;
            cout << "=================================================" << endl;
            showGroup(database);
            break;
        case 7:
            cout << "Ver todo" << endl;
            cout << "=================================================" << endl;
            listAll(database);
            break;
        case 8:
            cout << "Solicitar atención/asesoría académica" << endl;
            cout << "=================================================" << endl;
            requestAdvising(database);
            break;
        case 9:
            cout << "Atender siguiente solicitud" << endl;
            cout << "=================================================" << endl;
            attendNext(database);
            break;
        case 10:
            cout << "Cola de solicitudes pendientes" << endl;
            cout << "=================================================" << endl;
            showPendingQueue(database);
            break;
        case 11:
            cout << "Historial de entrada" << endl;
            cout << "=================================================" << endl;
            showEntryHistory(database);
            break;
        case 12:
            cout << "Lista de todas las solicitudes" << endl;
            cout << "=================================================" << endl;
            showAllRequests(database);
            break;
        case 0:
            clearRequests(database);
            cout << "Saliendo..." << endl;
            cout << "=================================================" << endl;
            return EXIT_SUCCESS;
        default:
            cout << "Opción errada" << endl;
            cout << "=================================================" << endl;
            break;
        }
    }
}