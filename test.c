/********************************************************/
/*Fichier “hello.c” */
/*Programme BONJOUR (Hello world, version fr ancaise) */
/*************** *****************************************/
main()
{
 char nom[]=”Anonyme”;
 int age;
 double salaire;
 char *univ = “ETS”;
 age = 30;
 salaire = 15000.00;
 printf (“Bonjour tout le monde, jai %d ans\n”, age);
 printf (“Mon nom est %s\n”, nom);
 printf (“et je gagne %f par mois\n”, salaire);
 printf (“Bonjour, mon nom est %s, j’ai %d ans et je gagne %.2f $ par an\n”,
 nom, age, salaire);
 printf (“J’enseigne a %s\n”, univ);
}