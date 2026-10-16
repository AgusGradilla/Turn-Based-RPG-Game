#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Creación de la estructura de Objetos con los datos que tiene
typedef struct {
    char object_name[20];
    int affected_attribute; // Este será el que eliga que atributo mejorará
    int power; // Acá elegira cuantos puntos subira
} Object;


// Creación dela estructura de Personajes con los datos que tiene
typedef struct {
    char character_name[20];
    int HP;
    int Damage;
    int physical_attack;
    int magical_attack;
    int physical_defense;
    int magical_defense;
    void (*magics[3])(void *, void *);// Se hace un arreglo de 3 punteros a función para las magias
    char *magic_names[3];
    int buff_turns;
    int lost_turn; // variables creadas para poder usarse en la lógica del programa (2 para magias y 1 para el combate)
    int evade;
    Object *inventory[3]; // Arreglo de apuntadores de tipo objeto
} Characters;

void clear_input_buffer(void){
    int character;
    while((character = getchar()) != '\n' && character != EOF);
}


// Creación de las magias
void Darkness_Shield(void *caster_void, void*target_void){ // Se crea la primer magia de tipo void que recibe de parametros 2 apuntadores genéricos
    Characters *p = (Characters *)caster_void; // Creamos un puntero a Characters y hacemos un cast
    if(p->buff_turns == 0){
        p->physical_defense +=4; // A traves del puntero entramos al valor de ambas defensas y subimos 4 puntos a c/u
        p->magical_defense += 4;
    }
    p->buff_turns = 3; // De la misma manera que hicimos con defensas entramos al valor de buff_turns y le asignamos 3 (El tiempo que dura el efecto)

    printf("\n====================================\n");
    printf("%s usa Darkness Shield\n", p->character_name);
    printf("Sus defensas aumentan 4 puntos\n");
    printf("Efecto dura 3 turnos\n");
    printf("====================================\n\n");

}

void Paralyze(void *caster_void, void*target_void){ // De igual manera que la anterior magia se crea un tipo void que recibe 2 apuntadores genéricos
    Characters *caster = (Characters *)caster_void; // Realizamos otro cast y hacemos un apuntador a la estructura Characters
    Characters *target = (Characters *)target_void; // Lo mismo que la línea de arriba

    if(caster->magical_attack > target->magical_defense) { // El if entra al valor de ambas instancias en la estructura y verifica que el ataque magico del caster sea mayor a la defensa del target
        target->lost_turn = 1; // Si cumple la condición entramos al valor de lost_turn del target y asignamos un 1
        printf("\n====================================\n");
        printf("%s ha paralizado a %s\n", caster->character_name, target->character_name);
        printf("%s pierde un turno\n", target->character_name);
        printf("====================================\n\n");
    } else {
        printf("\n====================================\n");
        printf("%s ha resistido la magia\n", target->character_name); // De ser contrario que la defensa sea mayor simplemente hacemos una impresión para avisar que no funcionó el atk magico
        printf("====================================\n\n");
    }

}

void Fire_Ball(void *caster_void, void*target_void){ // Estructura similar a las anteriores 2 magias con 2 punteros genéricos
    Characters *caster = (Characters *)caster_void; // Casteo a tippo Characters guardandolo en un puntero a Characters
    Characters *target = (Characters *)target_void; // lo mismo x2

    if(caster->magical_attack > target->magical_defense) { // If que verifica si el atk_magic del caster es mayor a la def_magic del target
        int Damage_Caused =  caster->magical_attack; // Creamos variable de tipo entero que guardará el valor del atk_magic del caster
        target->Damage += Damage_Caused; // El daño que reciba el target se incrementará dependiendo el valor de Damage_Caused

        if(target->Damage > target->HP){ // Verifica si el daño es mayor a la vida del enemigo si es así simplemente que el daño sea igual a su vida
            target->Damage = target->HP; // De esta manera evitamos valores negativos
        }

        printf("\n====================================\n");
        printf("%s ha lanzado Fire Ball a %s\n", caster->character_name, target->character_name);
        printf("%s ha recibido %d puntos de daño\n", target->character_name, Damage_Caused);
        printf("Vida restante de %s: %d\n", target->character_name, target->HP - target->Damage);
        printf("====================================\n\n");
    } else{ // De la misma manera esto solo imprime que no hizo daño el tak magico al tener mayor defensa el target
        printf("\n====================================\n");
        printf("Fire Ball ha fallado\n");
        printf("%s resistió el ataque sin daño\n", target->character_name);
        printf("====================================\n\n");
    }

}

// Magic Missile es la misma lógica que Fire ball así que me salto esta
void Magic_Missile(void *caster_void, void*target_void){
    Characters *caster = (Characters *)caster_void;
    Characters *target = (Characters *)target_void;

    if(caster->magical_attack > target->magical_defense) {
        int Damage_Caused = caster->magical_attack;
        target->Damage += Damage_Caused;

        if(target->Damage > target->HP){ // Verifica si el daño es mayor a la vida, en dado caso de ser verdad lo iguala a la vida
            target->Damage = target->HP; // Así se evita valores negativos
        }

        printf("\n====================================\n");
        printf("%s ha lanzado Magic Missile a %s\n", caster->character_name, target->character_name);
        printf("%s ha recibido %d puntos de daño\n", target->character_name, Damage_Caused);
        printf("Vida restante de %s: %d\n", target->character_name, target->HP - target->Damage);
        printf("====================================\n\n");
    } else {
        printf("\n====================================\n");
        printf("Magic Missile ha fallado\n");
        printf("%s resistio el ataque sin daño\n", target->character_name);
        printf("====================================\n\n");
    }
}    

void Heal(void *caster_void, void*target_void){ // Otra función que recibe 2 apuntadores genéricos
    Characters *p = (Characters *)caster_void; // De misma forma hacemos un cast a tipo Characters guardandolo en un puntero a Characters
    int heal_pts = 5; // Hacemos una variable entera que tenga el valor de 5 (Será la curación base)
    int super_heal = rand() % 15; // De la misma manera hacemos otra variable de tipo entero que tendrá un randomizador en su valor de 0 a 14

    if(super_heal == 0){ // Comprobamos si la variable con el random obtiene un valor de 0
        heal_pts = 20; // Si es el caso la curación base se sobreescribe a 20
        printf("\n====================================\n");
        printf("La diosa Aqua ha escuchado tus plegarias y te ha curado toda la vida\n");
        printf("Recibes curación máxima\n");
        printf("====================================\n\n");
    }

    p->Damage -= heal_pts; // Restamos al daño el valor de la curación

    if(p->Damage < 0) { // Si el valor del daño resulta ser negativo  lo volvemos a incializar a 0
        p->Damage = 0;
    }

    printf("\n====================================\n");
    printf("%s uso Heal\n", p->character_name);
    printf("Recupera %d puntos de vida\n", heal_pts);
    printf("Vida restante: %d\n", p->HP - p->Damage);
    printf("====================================\n\n");

}


// Se inicializa los atributos del personaje
void Initialize_character(Characters *p, const char *name, int hp, int pa, int ma, int pd, int md){ // Puntero a Characters, puntero de una cadena constante, las demas se usarán para poder modificar los atributos del personaje
    strncpy(p->character_name, name, sizeof(p->character_name) - 1);
    p->character_name[sizeof(p->character_name) - 1] = '\0';
    p->HP = hp;
    p->Damage = 0;
    p->physical_attack = pa;
    p->magical_attack = ma;
    p->physical_defense = pd;
    p->magical_defense = md;
    p->buff_turns = 0;
    p->lost_turn = 0;
    p->evade = 0;
    for(int i = 0; i < 3; i++) { // Recorre el inventario del persoanje inicializandolo en NULL (Eso es porque al inicio no tiene nada)
        *(p->inventory + i) = NULL;
     }
}

Object *aleatory_object(){
    Object *new_object =(Object *)malloc(sizeof(Object)); // Se crea de manera dinamica un espacio en memoria del tamaño de Object guardandolo en un puntero a Object (También realizamos el cast para tratarlo como Objetct)
    if(new_object == NULL) { // Verifica si el sistema quedo sin memoria en dado caso salimos del programa
        printf("Error: No hay espacio para el objeto\n");
        exit(1);
    }

    char *object_names[] = {"Poción", "Excalibur", "Lanza de Longinus", "Incursio", "Amuleto mágico"}; // Arreglo de puntero de caracteres que guardara los nombres de los objetos

    int name_index = rand() % 5; // Variable de tipo entero que tendrá un valor de 0-4 dependiendo el randomizador
    strcpy(new_object->object_name, *(object_names + name_index)); // Copia el nombre del objeto en el indice que dio la varibale name_index a la instancia object name accediendolo a traves del puntero new_object anteriormente creado

    switch(name_index){ // switch case que recibe el valor del name_index
        case 0:
            new_object->affected_attribute = 0; // En este switch dependiendo el objeto accemos al valor a modificar (affected_atribute) y asignamos cuanto lo modificara (power)
            new_object->power = 10;
            break;
        case 1:
            new_object->affected_attribute = 1;
            new_object->power = 5;
            break;
        case 2:
            new_object->affected_attribute = 1;
            new_object->power = 3;
            break;
        case 3:
            new_object->affected_attribute = 3;
            new_object->power = 5;
            break;
        case 4:
            new_object->affected_attribute = 2;
            new_object->power = 5;
            break;
    }
    return new_object; // Devolvemos el puntero del objeto creado
}

// Misma lógica explicada en Initialize Characters (la primer parte)
void stats_enemies(Characters * p, const char *name, int hp, int pa, int pd, int ma, int md){
    strcpy(p->character_name, name);
    p->HP = hp;
    p->Damage = 0;
    p->physical_attack = pa;
    p->physical_defense = pd;
    p->magical_attack = ma;
    p->magical_defense = md;
    p->buff_turns = 0;
    p->lost_turn = 0;
    p->evade = 0;
    for(int i = 0; i < 3; i++){
        *(p->inventory + i) = NULL;
    }
}

char *game_magics_names[] = {"Darkness Shield", "Paralyze", "Fire Ball", "Magic Missile", "Heal"}; // Arreglo de punteros a String donde cada elemento es el nombre de las magias
void (*game_magics[])(void*, void*) = {Darkness_Shield, Paralyze, Fire_Ball, Magic_Missile, Heal}; // Este es un punto a función que toma 2 elementos como argumentos, se creo para acceder a traves del indice de la estructura anterior a la magia seleccionada

void Inizialize_enemies(Characters *enemies) { // Función que recibe un puntero a Characters
 int num_magics = 5; // variable de tipo entero que guarda el numero de magias que hay (5)
 int magic_index[5]; // Arreglo de tipo entero que tiene 5 espacios de memoria

 int enemies_objects[] = {0, 1, 3, 3}; // El inventario tiene 3 espacios como máximo

// Llamamos a la función stats_enemies donde dependiendo su indice le asignamos el nombre y las stats que contendrán
stats_enemies((enemies + 0), "Kobold", 10, 6, 4, 4, 4);
stats_enemies((enemies + 1), "Dark Elf", 15, 6, 6, 6, 6);
stats_enemies((enemies + 2), "Dragón", 20, 7, 7, 7, 7);
stats_enemies((enemies + 3), "Rey Demonio", 30, 10, 10, 10, 10);

// Bucle for que se recorrerá para cada uno de los 4 enemigos
 for(int i = 0; i < 4; i++){

    // Este recorrera el numero de magias que existene en el jeugo
    for(int k = 0; k < num_magics; k++){
        *(magic_index + k) = k; // Aquí funciona como una lista de indices para las magias
    }
    // Este bucle solo se recorrerá 3 veces para seleccionar 3 magias por enemigo
    for(int j = 0; j < 3; j++){
        int aleatory_index = rand() % (num_magics - j); // Agregamos una variable de tipo entero la cual decidira el indice de la magia de manera aleatoria
        int index_random_magic = magic_index[aleatory_index]; // Nueva variable entera que contendrá el indice de la magia 
        
        (enemies + i)->magics[j] = game_magics[index_random_magic]; // Asignamos el puntero al indice de la magia seleccionada para el enemigo
        (enemies + i)->magic_names[j] = game_magics_names[index_random_magic]; // De la misma forma asignamos el nombre correspondiente a traves del indice
        
        int temp = *(magic_index + aleatory_index);
        *(magic_index + aleatory_index) = magic_index[num_magics -1 -j];
        *(magic_index + num_magics -1 -j) = temp;
    }

    // Bucle for que iterara las veces de objetos que tendrán los enemigos
    for(int j = 0; j < enemies_objects[i]; j++){
        (enemies + i)->inventory[j] = aleatory_object(); // El objeto aleatorio lo decidira la función aleatory_object guardandolo en el inventario del enemgio
    }
 }
}

// Se creo esta función para que itere un arreglo de punteros a Object "number" veces (Objetos esperados a liberar)
void free_objects(Object **Objects, int number) {
    for(int i = 0; i < number; i++){
        if(*(Objects + i) != NULL) { // Comprobamos que el valor del objeto no sea NULL para evitar liberar otro espacio que no queremos
            free(*(Objects + i)); // Hacemos free al Objeto
            *(Objects + i) = NULL; // Y volvemos a establecerlo en NULL
        }
    }
}


void turn_effects(Characters *p){ // Función que recibe un puntero a Characters
    if(p->buff_turns > 0){ // A traves de un if comprobamos si el valor de buff_turn de la estructura Characters 
        p->buff_turns--; // Y vamos decrementando en 1 el valor de buff_turn
        
        if(p->buff_turns == 0){ // Este if comprobara si el valor de buff_turn ya es igual a 0
            printf("\n====================================\n");
            printf("El efecto de Darkness Shield ha acabado\n");
            printf("Defensas vuelven a la normalidad\n");           // Damos mensaje de que el efecto a terminado
            printf("====================================\n\n");
            p->physical_defense -= 4; // Volvemos a bajar las defensas a la normalidad
            p->magical_defense -= 4;
        }
    }
}

int player_inventory(Characters *player){ // Función que recibe un puntero a la estructura Characters (Será lo más común en el código 1 o 2 punteros a Characters)
    int choice; // variable de tipo entero para que elija el objeto a usar
    int items = 0; // Inicializamos la variable items en 0

    printf("\n====================================\n");
    printf("          INVENTARIO\n");
    printf("====================================\n");
    for(int i = 0; i < 3; i++){ // Ciclo for que recorre los 3 espacios del inventario
        if(*(player->inventory + i) != NULL){ // Verifica si el espacio actual del inventario no es NULL (Si no esta vacío)
            printf("%d) %s (beneficio: +%d)\n", i + 1, (*(player->inventory + i))->object_name, (*(player->inventory + i))->power); // Por cada objeto imprimimos su indice del inventario, nombre y cantidad de beneifico
            items++; // Y vamos aumentando la cantidad de items
        }
    }

    if(items == 0){ // Si no hubo items en el inventario simplemente imprimimos que esta vacío
        printf("Inventario vacío\n");
        printf("====================================\n\n");
        return 0; // Hacemos que si el inventario es 0 regrese 0 (Para fines practicos en el player_turn allá exlico más)
    }

    // Le preguntamos al jugador que objeto queremos usar y guardamos el resultado en choice
    printf("====================================\n");
    printf("¿Qué objeto quieres usar? ");
    if(scanf("%d", &choice) != 1){
        clear_input_buffer();
        printf("Opción inválida\n");
        return 0;
    }

    // Comprobamos que que la selección este dentro del rango de 1 a 3 y a su vez verificamos que el espacio si tenga un objeto (Que no sea NULL)
    if(choice >= 1 && choice <= 3 && *(player->inventory + (choice - 1)) != NULL){
        Object *used_item = *(player->inventory + (choice - 1)); // Si pasa la comprobación creamos un puntero a Object para poder referenciar el objeto elegido

        // Este switch se a traves del valor que afectara el objeto (HP, Pshy/mgc_atk, mgc/pshy_def) el objeto que se usará
        printf("\n====================================\n");
        switch(used_item->affected_attribute){ 
            case 0: // Si es 0 (HP) hacemos el Daño del jugador se le reste el poder del objeto
                player->Damage -= used_item->power;
                if(player->Damage < 0) player->Damage = 0; // Si el daño es menor a 0 lo volvemos a poner como 0 (Evitamos daño negativo)
                printf("%s ha usado %s\n", player->character_name, used_item->object_name);
                printf("Recupera %d de HP\n", used_item->power); // E imprimimos lo que sucede
                printf("Vida restante: %d\n", player->HP - player->Damage);
                break;
            case 1: // De la misma forma del anterior le subimos la estadistica de ataque físico e imprimimos
                player->physical_attack += used_item->power;
                printf("%s ha usado %s\n", player->character_name, used_item->object_name);
                printf("Ataque físico aumenta +%d puntos\n", used_item->power);
                break;
            case 2: // Igual subimos el ataque magico e imprimimos
                player->magical_attack += used_item->power;
                printf("%s ha usado %s\n", player->character_name, used_item->object_name);
                printf("Ataque mágico aumenta +%d puntos\n", used_item->power);
                break;
            case 3: // Subimos ambas defensas e imprimimos
                player->physical_defense += used_item->power;
                player->magical_defense += used_item->power;
                printf("%s ha usado %s\n", player->character_name, used_item->object_name);
                printf("Ambas defensas aumentan +%d puntos\n", used_item->power);
                break;
        }
        printf("====================================\n\n");

        free(used_item); // Liberamos la memoria ya que el objeto ya fue usado
        *(player->inventory + (choice - 1)) = NULL; // Y establecemos la memoria del inventario (del objeto usado) como NULL
        return 1; // Si llego a usar un objeto regresamos 1
    } else{ // Se ejecuta si elige una opción equivocada
        printf("\n====================================\n");
        printf("Opción inválida\n");
        printf("====================================\n\n");
        return 0; // Si uso una opción inválida regresamos 0
    }
}

void add_item(Characters *player, Object *new_item){ // Función que recibe un puntero a Characters y otro puntero a Object
    int free_slot = -1; // Creamos una nueva variable y la inicializamos en -1
    int choice; // Creamos una variable de tipo entera que contendrá el valor que pondrá el usuario
    
    for(int i = 0; i < 3; i++){ // For que recorre de hasta 3 (valor del inventario)
        if(*(player->inventory + i) == NULL){ // Revisa si el valor en la posición i del inventario es NULL (Si esta vacío)
            free_slot = i; // Si esta vacío asignamos el valor de i a free_slot
            break;
        }
    }
    
    if(free_slot != -1){ // if que verifica si el valor de free_slot es diferente de 1
        *(player->inventory + free_slot) = new_item; // Añadimos el nuevo objeto en la posición de free slot
        printf("\n====================================\n");
        printf("%s ha añadido %s al inventario\n", player->character_name, new_item->object_name);
        printf("====================================\n\n");
        
        switch(new_item->affected_attribute){ // Switch para evaluar que atributoa fectará el nuevo objeto
            case 0:
                player->HP += new_item->power;
                break;
            case 1:
                player->physical_attack += new_item->power;
                break;
            case 2:
                player->magical_attack += new_item-> power;
                break;
            case 3:
                player->physical_defense += new_item-> power;
                player->magical_defense += new_item-> power;
                break;
        }
    } else { // Si el inventario ya estaba lleno mandamos mensaje para eliminar 1 objeto
        printf("\n====================================\n");
        printf("INVENTARIO LLENO\n");
        printf("Elige un objeto para eliminar:\n");
        printf("====================================\n");
        
        for(int i = 0; i < 3; i++){ // Volvemos a recorrer los 3 objetos del inventario
            printf("%d) %s (beneficio: +%d)\n", i + 1, (*(player->inventory + i))->object_name, (*(player->inventory + i))->power);
        }
        
        printf("====================================\n");
        printf("Selecciona el objeto que deseas eliminar: ");
        if(scanf("%d", &choice) != 1){ // Escaneamos la opción del jugador
            clear_input_buffer();
            free(new_item);
            printf("Opción inválida\n");
            return;
        }

        if(choice >= 1 && choice <= 3){ // Verificamos que el valor este dentro del rango de 1 a 3
            Object *remove_item = *(player->inventory + (choice -1)); // Creamos un puntero a Object que guardará el item a borrar
    
            if(strcmp(remove_item->object_name, "Poción") == 0){     // Comparamos los string del objeto eliminado y el nombre de este mismo, borrando el efecto de los atributos usados
            } else if (strcmp(remove_item->object_name, "Excalibur") == 0 ){
                player->physical_attack -=5;
            } else if (strcmp(remove_item->object_name, "Lanza de Longinus") == 0 ){
                player->physical_attack -= 3;
            } else if(strcmp(remove_item->object_name, "Incursio") == 0 ){
                player->physical_defense -= 5;
                player->magical_defense -= 5;
            } else if(strcmp(remove_item->object_name, "Amuleto mágico") == 0 ){
                player->magical_attack -= 5;
            }
            
            free(remove_item); // Liberamos el espacio de memoria del atributo que quitamos
            
            *(player->inventory + (choice - 1)) = new_item;
            printf("\n====================================\n");
            printf("%s añadido al inventario\n", new_item->object_name); // Mensaje de que se guardo en el inventario
            printf("====================================\n\n");
            
            switch(new_item->affected_attribute){ // switch que añade al nuevo objeto la cantidad de puntos del atributo que mejorara
                case 0:
                    player->HP += new_item->power;
                    break;
                case 1:
                    player->physical_attack += new_item->power;
                    break;
                case 2:
                    player->magical_attack += new_item-> power;
                    break;
                case 3:
                    player->physical_defense += new_item-> power;
                    player->magical_defense += new_item-> power;
                    break;
            }
        } else {
            free(new_item);
            printf("Opción inválida\n");
        }
    }
}

void rewards(Characters *player) { // función que recibe de parametros un puntero a la estructura Characters
    printf("\n====================================\n");
    printf("          RECOMPENSAS\n");
    printf("====================================\n");
    printf("%s ha encontrado un tesoro\n", player->character_name);
    printf("Puede llevarse 2 objetos como recompensa\n");
    printf("====================================\n\n");
    
    Object *battle_rewards[4]; // Declaramos un arreglo de punteros de tipo Object que pueda almacenar 4 objetos

    for(int i = 0; i < 4; i++) { // Ejecutamos un bucle que iterará 4 veces (El mismo numero de veces que creará objeto)
        *(battle_rewards + i) = aleatory_object(); // Usamos la función de aleatory_object que crea un objeto de manera dinamica y la guardamos en uno de los indices de battle_rewards
        printf("%d) %s (Poder: +%d)\n", i + 1, (*(battle_rewards + i))->object_name, (*(battle_rewards + i))->power);
    }

    for(int i = 0; i < 2; i++){ // for que iterara 2 veces (solo puedes elegir 2 objetos de 4)
        int choice;
        printf("\nSelecciona el objeto %d que quieres: ", i + 1);
        if(scanf("%d", &choice) != 1){ // verificamos que sea numero válido mientras leemos la opción
            clear_input_buffer();
            i--;
            continue;
        }

        if(choice >= 1 && choice <= 4 && *(battle_rewards + (choice - 1)) != NULL){ // Verificamos que choice este entre el rango (de 1 a 4) y que el objeto no sea NULL
            add_item(player, *(battle_rewards + (choice -1))); // añadimos el objeto al inventario con la función anterior
            
            *(battle_rewards + (choice -1)) = NULL; // Asignamos como NULL el puntero del objeto que seleccionamos
        } else {
            printf("\n====================================\n");
            printf("Opción inválida, intente de nuevo\n");
            printf("====================================\n\n");
            i--; // Si elige una opción invalida mandamos mensaje de error y con i-- repetimos la interacción
        }
    }
    
    for(int i = 0; i < 4; i ++){ // For que recorre los 4 objetos creados
        if(*(battle_rewards + i) != NULL){ // Verificamos que el objeto no sea NULL y lo liberamos
            free(*(battle_rewards + i));
        } 
    }
}

void magics_player(Characters *player, Characters *enemy){ // Función que recibe 2 punteros a la estructura Characters
    printf("\n====================================\n");
    printf("          SELECCIÓN DE MAGIA\n");
    printf("====================================\n");
    printf("Elige tu magia:\n");
    for(int i = 0; i < 3; i++){ // Hacemos un bucle que iterara 3 veces (Las magias que tiene el jugador)
        printf("%d) %s\n", i+1, *(player->magic_names + i));
    }
    printf("====================================\n");
    printf("Opción: ");
    
    int choice;
    if(scanf("%d", &choice) != 1){
        clear_input_buffer();
        return;
    }
    
    if(choice >= 1 && choice <= 3){ // Verificamos que que la opción sea entre el rango válido (1 a 3)
        void (*magic_use)(void*, void*) = player->magics[choice - 1]; // Creamos un puntero a función llamado magic_use que recibirá 2 parametros de tipo void y la igualamos al indice de magics (magia a usar)
        magic_use(player, enemy); // Llamamos a la función magic_use
    }
}

void evade_attack(Characters *p){ // Función que recibe de parametros un puntero a Characters
    if(rand() % 2 == 0){ // Hacemos un if que comprobara un randomizer de 0 y 1
        p->evade = 1; // Si el randomizer entramos a traves del puntero el valor de evade y le asignamos 1
        printf("\n====================================\n");
        printf("%s se prepara para evadir\n", p->character_name);
        printf("Bloqueará el siguiente ataque del rival\n");
        printf("====================================\n\n");
    } else {
        p->evade = 0; // De lo contrario le asignamos 0
        printf("\n====================================\n");
        printf("%s intentó evadir pero falló\n", p->character_name);
        printf("====================================\n\n");
    }
}

void physical_attack(Characters *attacker, Characters *Opponent){ // Función que recibe 2 punteros a la estructura Charactrers
    if(Opponent->evade == 1){ // Antes que nada verificamos si el evade del oponente es 1,
        printf("\n====================================\n"); // En dado caso de ser cierto reescribimos evade en 0 y retornamos
        printf("%s ha evadido el ataque exitosamente\n", Opponent->character_name);
        printf("====================================\n\n");
        Opponent->evade = 0;
        return; // De esta manera evitamos que ataque
    }
    
    // Creamos la variable Damage_Caused que será la diferencia entre el ataque físico del atacante menos la diferencia de la defensa fisica del oponente
    int Damage_caused = attacker->physical_attack -  Opponent->physical_defense; 
    if(Damage_caused < 0){ // Si el daño causado es menor a 0 lo reescribimos  0
        Damage_caused = 0;
    }

    Opponent->Damage += Damage_caused; // El Daño del oponente se aumentará al daño causado

    if(Opponent->Damage > Opponent->HP){
        Opponent->Damage = Opponent->HP;
    }

    int remaining_HP = Opponent->HP - Opponent->Damage; // Parecido a la lógica para que no haya vida restante negativa de las magias
    if(remaining_HP < 0){ // Como no me funciono la misma comprobación que antes tuve que implementar esta
        remaining_HP = 0;
    }
    
    printf("\n====================================\n");
    printf("%s atacó a %s\n", attacker->character_name, Opponent->character_name);
    printf("Daño causado: %d puntos\n", Damage_caused);
    printf("Vida restante de %s: %d\n", Opponent->character_name, remaining_HP);
    printf("====================================\n\n");
}

void use_item_enemies(Characters *enemy, Characters *player) { // Función que recibe 2 punteros a Characters
    int item_index = -1; // Creamos una variable y la inicializamos en -1
    
    for (int i = 0; i < 3; i++) { // Bucle que recorre los 3 elementos del inventario
        if (*(enemy->inventory + i) != NULL) { // Verificamos que el espacio en el inventario no sea null (Que haya un objeto)
            item_index = i; // Gurdamos el indice del objeto 
            break; 
        }
    }

    if (item_index != -1) { // Verificar si el index del objeto no es -1
        Object *item = *(enemy->inventory + item_index); // Creamos un puntero a Object y lo igualamos al objeto del inventario del enemigo
        printf("\n====================================\n");
        printf("%s usa el objeto: %s (Poder: %d)\n", enemy->character_name, item->object_name, item->power);
        printf("====================================\n\n");

        switch (item->affected_attribute) { // Switch que verá el atributo que afecto el objeto
            // Por cada caso simplemente aplicamos los respectimos buffos y tiramos el mensaje de que uso
            case 0:
                enemy->Damage -= item->power;
                if (enemy->Damage < 0) enemy->Damage = 0;
                printf("%s se ha curado, vida restante: %d\n", enemy->character_name, enemy->HP - enemy->Damage);
                break;
            case 1:
                enemy->physical_attack += item->power;
                printf("El ataque fisico de %s ha aumentado %d puntos\n", enemy->character_name, item->power);
                break;
            case 2:
                enemy->magical_attack += item->power;
                printf("El ataque magico de %s ha aumentado %d puntos\n", enemy->character_name, item->power);
                break;
            case 3:
                enemy->physical_defense += item->power;
                enemy->magical_defense += item->power;
                printf("Las defensas de %s han aumentado %d putos\n", enemy->character_name, item->power);
                break;
        }

        // Liberar la memoria del objeto usado
        free(item);
        *(enemy->inventory + item_index) = NULL;
        
    } else { // Si el enemigo intenta usar objeto pero no tiene hacemos que use un ataque fisico
        printf("%s Intento usar un objeto pero no tiene, procede a atacar\n", enemy->character_name);
        physical_attack(enemy, player);
    }
}

void player_turn(Characters *player, Characters *enemy){ // Función que recibe 2 punteros a Characters
    int action_consumed = 0; // Hacemos una flag para determinar si el turno acabo
    int choice;
    
    do { // bucle do-while (Sabemos que el do al menos se ejecutará 1 vez exitosamente)
        // "Menú" que nos dirá que queremos hacer en nuestro turno
        printf("\n====================================\n");
        printf("       TURNO DE %s\n", player->character_name);
        printf("====================================\n");
        printf("1. Ataque Físico\n");
        printf("2. Magia\n");
        printf("3. Objetos\n");
        printf("4. Evasión\n");
        printf("====================================\n");
        printf("Elige tu acción: ");
        
        if(scanf("%d", &choice) != 1){
            clear_input_buffer();
            continue;
        }
        
        switch(choice){ // Switch que recibe el valor de la acción que queremos realizar (choice)
        // Dependiendo los casos mandamos a llamar a las funciones que se encarga de cada cosa
            case 1:
                physical_attack(player, enemy);
                action_consumed = 1;
                break;
            case 2:
                magics_player(player, enemy);
                action_consumed = 1;
                break;
            case 3:
                action_consumed = player_inventory(player); // Igualamos la flag a la función de player_inventory que retorna un numero, que puede ser 0 o 1 dependiendo el caso (sigo abajo)
                break; // Si es 0 significa que el usuario tenía el inventario vacío o uso una opción incorrecta (No le consumimos turno) o 1 si uso objeto (consumimos turno)
            case 4:
                evade_attack(player);
                action_consumed = 1;
                break;
            default: // simplemente el usuario puso opción incorrecta y sigue su turno hasta que elija bien
                printf("Opción incorrecta elige otra\n");
                action_consumed = 0;
                break;
        }
    } while(action_consumed == 0); // El bucle se correrá hasta que while sea diferente de 0
}

void enemy_turn(Characters *enemy, Characters *player){ // Función que recibe 2 punteros a función
    printf("\n====================================\n");
    printf("       TURNO DE %s\n", enemy->character_name);
    printf("====================================\n");

    if(enemy->lost_turn > 0){ // Verificamos si lost turn del enemigo es mayor a 0, en dado caso imprimimos lo sucedido
        printf("%s está paralizado y no puede moverse\n", enemy->character_name);
        printf("====================================\n\n");
        enemy->lost_turn = 0; // Modificamos lost_turn en 0 y regresamos
        return;
    }
    
    int choice = rand() % 4; // Agregamos la variable choice que sera un rand entre 0 a 3

    switch(choice){ // Un switch que recibe el valor de choice
        // Dependiendo el valor de rand será el caso a elegir y simplemente llamamos la función que corresponde
        case 0:
            physical_attack(enemy, player);
            break;
        case 1:
            enemy->magics[rand() % 3](enemy, player);
            break;
        case 2:
            evade_attack(enemy);
            break;
        case 3:
            use_item_enemies(enemy, player);
            break;
    }
}

void battle (Characters *player, Characters *enemy){ // Funcion que recibe 2 punteros a Characters
    // imprimos el mensaje de inicio de batalla
    printf("\n====================================\n");
    printf("          COMENZANDO BATALLA\n");
    printf("====================================\n");
    printf("%s se enfrenta a %s\n", player->character_name, enemy->character_name);
    printf("de las fuerzas del Señor Oscuro\n");
    printf("====================================\n\n");
    
    // Implementamos a traves de un while la lógica que seguira el combate (Que la vida del jugador o la del enemigo sea mayor 0)
    while(player->HP - player->Damage > 0 && enemy->HP - enemy->Damage > 0){
        player_turn(player, enemy); // Mandamos a llamar la función de turno del jugador
        turn_effects(enemy); // Mandamos a llamar a la función de efecto de turno de enemigos

        // Verificamos si la vida del enemigo es menor o igual a 0 y en caso de ser cierto imprimimos el mensaje de victoria
        if(enemy->HP - enemy->Damage <= 0){
            printf("\n====================================\n");
            printf("          VICTORIA\n");
            printf("====================================\n");
            printf("%s ha ganado la batalla\n", player->character_name);
            printf("%s ha sido derrotado\n", enemy->character_name);
            printf("====================================\n\n");
            break;
        }

        enemy_turn(enemy,player); // Mandamos a llamar la función de turno del enemigo
        turn_effects(player); // Y de la misma manera llamamos los efectos de turno del jugador

        // Verificar que la vida del jugador sea menor o igual a 0, en caso de ser cierto imprimimos el mensaje de derrota
        if(player->HP - player->Damage <= 0) {
            printf("\n====================================\n");
            printf("          DERROTA\n");
            printf("====================================\n");
            printf("%s te ha derrotado\n", enemy->character_name);
            printf("FIN DEL JUEGO\n");
            printf("====================================\n\n");
        }
    }
}

int main() {
    srand(time(NULL)); // Inicializamos el generador de numeros aleatorios
    Characters players[5]; // Arreglo de tipo Characters (El jugador y los enemigos)
    char name_buffer[50]; // Arreglo de string que contendrá el nombre del jugador
    int choice;

    // Inicializamos las estadisticas del jugador
    int player_hp = 20;
    int player_pa = 5;
    int player_ma = 5;
    int player_pd = 5;
    int player_md = 5;

    printf("================================================================================\n");
    printf("                  ISEKAI GENERICO GAME\n");
    printf("================================================================================\n");
    printf("Bienvenido, mi nombre es Aqua la diosa de este mundo.\n");
    printf("Te he traído para que liberes el mundo de las garras del malvado Rey Demonio.\n");
    printf("Así que dime, héroe...\n");
    printf("================================================================================\n\n");

    printf("¿Cuál es tu nombre? ");
    fgets(name_buffer, sizeof(name_buffer), stdin); //  Recibe el nombre del usuario
    name_buffer[strcspn(name_buffer, "\n")] = 0;
    printf("====================================\n\n");

    // Llama a la función encargada de inicializar el jugador
    Initialize_character(&players[0], name_buffer, player_hp, player_pa, player_ma, player_pd, player_md);
    
    printf("====================================\n");
    printf("          SELECCIÓN DE MAGIAS\n");
    printf("====================================\n");
    printf("Te daré 3 magias para que puedas derrotarlo.\n");
    printf("Selecciona las que quieras:\n");
    printf("====================================\n");
    // For que iterara las 5 magias imprimiendolas
    for (int i = 0; i < 5; i++) {
        printf("%d) %s\n", i + 1, game_magics_names[i]);
    }
    printf("====================================\n");
    
    // for que se ejecutara 3 veces (para que elija las 3 magias)
    for (int i = 0; i < 3; i++) {
        printf("Selecciona la Magia %d: ", i + 1);
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) { // Comprobamos que la elección del usuario este dentro del rango
            printf("Elección inválida, selecciona una correcta\n");
            i--; // Regresamos hasta que elija uno valido
            while(getchar() != '\n'); // Limpia el búfer
            continue;
        }
        players[0].magics[i] = game_magics[choice - 1]; // Accedemos al arreglo de magias del jugador igualandolo a la variable game_magics(en el indice que selecciono su magia)
        players[0].magic_names[i] = game_magics_names[choice - 1]; // De igual manera pero accediendo a los nombres de las magias
    }
    while(getchar() != '\n'); 

    // Llamamos a la función encargada de incializar a los enemigos a partir del indice 1 del arreglo de players (el enemigo fácil)
    Inizialize_enemies(&players[1]); 

    printf("\n================================================================================\n");
    printf("                 LA AVENTURA DEL HEROE A COMENZADO\n");
    printf("================================================================================\n\n");

    // búcle for que iterara 4 veces (para los enemigos)
    for (int i = 0; i < 4; i++) {
        Characters *current_enemy = &players[1 + i]; // creamos un puntero a la estructura Characters que guardará la dirección de memoria del enemigo actual

        // Mandamos a llamar la función de battle pasandole los parametros del jugador y enemigo actual
        battle(&players[0], current_enemy);

        // Verificamos si el hp del jugador es menor o igual a 0 y damos el mensaje de derrota
        if (players[0].HP - players[0].Damage <= 0) {
            printf("\n================================================================================\n");
            printf("                    GAME OVER\n");
            printf("================================================================================\n");
            printf("Has caído ante el ejército del Rey Demonio.\n");
            printf("La aventura ha terminado.\n");
            printf("================================================================================\n\n");
            break; 
        }

        // verificamos si hemos ido derrotando a los enemigos y damos el mensaje de victoria
        if (i < 3) {
            printf("\n================================================================================\n");
            printf("                    VICTORIA\n");
            printf("================================================================================\n");
            printf("¡%s ha ganado la batalla!\n", players[0].character_name);
            printf("================================================================================\n\n");
            rewards(&players[0]);
        } else { // Si no es 0, 1 o 2 significa que derrotamos al jefe final y da el mensaje de victoria
            printf("\n================================================================================\n");
            printf("                    FINAL\n");
            printf("================================================================================\n");
            printf("¡FELICIDADES HÉROE!\n");
            printf("Has derrotado al Rey Demonio y salvado el mundo.\n");
            printf("================================================================================\n\n");
        }
    }

    // Al final liberamos la memoria de los objetos
    for (int i = 0; i < 5; i++) {
         free_objects(players[i].inventory, 3);
    }
    
    return 0;
}