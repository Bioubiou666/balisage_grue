/*
#include <wiringPi.h>
#include <stdio.h>
#include <string>

using namespace std;


#define LedPin		0
#define ButtonPin 	1


bool writeToDatabase(const std::string& value1, const std::string& value2) {
    // Initialise la connexion
    MYSQL *conn;
    conn = mysql_init(NULL);

    string localhost = "localhost";
    string username = "username";
    string password = "password";
    string database_name = "database_name";

    if (conn == NULL) {
        std::cerr << "Erreur d'initialisation de la connexion : " << mysql_error(conn) << std::endl;
        return false;
    }

    // Connecte à la base de données
    if (mysql_real_connect(conn, localhost, username, password, database_name, 0, NULL, 0) == NULL) {
        std::cerr << "Erreur de connexion : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // Prépare la requête SQL
    std::string query = "INSERT INTO table_name (column1, column2) VALUES ('" + value1 + "', '" + value2 + "')";

    // Exécute la requête SQL
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Erreur d'exécution de la requête : " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }

    // Ferme la connexion
    mysql_close(conn);

    std::cout << "Données insérées avec succès !" << std::endl;
    return true;
}





int main(void){
	// Lorsque le câblage d'initialisation échoue, imprimez le message à l'écran
	if(wiringPiSetup() == -1) {
		printf("setup wiringPi failed !");
		return 1; 
	}
	
	pinMode(LedPin, OUTPUT); 
	pinMode(ButtonPin, INPUT);
	digitalWrite(LedPin, HIGH);

    writeToDatabase("valeur1", "valeur2");
	
	while(1) {
		// Indique que le bouton est enfoncé
		if(digitalRead(ButtonPin) == 0) {
			// Led allume
			digitalWrite(LedPin, LOW);
		//	printf("...LED on\n");
		}
		else {
			// Led eteint
			digitalWrite(LedPin, HIGH);
		//	printf("LED off...\n");
		}
	}
	return 0;
}
*/

#include <wiringPi.h>       // Pour utiliser les GPIO du Rasp
#include <stdio.h>
#include <string>

using namespace std;


#define LedPin		0
#define ButtonPin 	1


bool writeToDatabase(const string& value1, const string& value2) {
    // Initialise la connexion
    MYSQL *conn;
    conn = mysql_init(NULL);

    string localhost = "localhost";
    string username = "root";
    string password = "root";
    string database_name = "database_name";

    string table_name = "table_name";

    if (conn == NULL) {
        cerr << "Erreur d'initialisation de la connexion : " << mysql_error(conn) << endl;
        return false;
    }

    // Connecte à la base de données
    if (mysql_real_connect(conn, localhost, username, password, database_name, 0, NULL, 0) == NULL) {
        cerr << "Erreur de connexion : " << mysql_error(conn) << endl;
        mysql_close(conn);
        return false;
    }

    // Prépare la requête SQL
    string query = "INSERT INTO " + table_name + " (column1, column2) VALUES ('" + value1 + "', '" + value2 + "')";

    // Exécute la requête SQL
    if (mysql_query(conn, query.c_str()) != 0) {
        cerr << "Erreur d'exécution de la requête : " << mysql_error(conn) << endl;
        mysql_close(conn);
        return false;
    }

    // Ferme la connexion
    mysql_close(conn);

    cout << "Données insérées avec succès !" << endl;
    return true;
}





int main(void){
	/*
    // Lorsque le câblage d'initialisation échoue, imprimez le message à l'écran
	if(wiringPiSetup() == -1) {
		printf("setup wiringPi failed !");
		return 1;
	}
	
	pinMode(LedPin, OUTPUT); 
	pinMode(ButtonPin, INPUT);
	digitalWrite(LedPin, HIGH);

    writeToDatabase("valeur1", "valeur2");
	
	while(1) {
		// Indique que le bouton est enfoncé
		if(digitalRead(ButtonPin) == 0) {
			// Led allume
			digitalWrite(LedPin, LOW);
		//	printf("...LED on\n");
		}
		else {
			// Led eteint
			digitalWrite(LedPin, HIGH);
		//	printf("LED off...\n");
		}
	}
    */

    while (1) {      // Boucle infinie

        while (1) {      // Branché sur secteur
            zzz
        }
    }
	return 0;
}
