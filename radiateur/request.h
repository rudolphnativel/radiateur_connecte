

/**
 * renvoie toutes les données de tous les module
 */
void GetAll( WiFiClientSecure client, EspUser esp);

/**
 * renvoie EspUser si le module a déjà un utilisateur, et un EspUser vide sinon
 */
EspUser GetOne(WiFiClientSecure client, EspUser esp);

/**
 * renvoie EspUser si le module a bien été associer à l'utilisateur, et un EspUser vide sinon
 */
EspUser Add(WiFiClientSecure client, EspUser esp) ;

/**
 * renvoie EspUser si le module a bien été associer à l'utilisateur
 * synchronie avec la base de données en fonction des information s
 */
EspUser GetStatus(WiFiClientSecure client, EspUser esp);
