#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include <time.h>
#include "Tree.h"
#include <string.h>
#include "sys/times.h"
#include <unistd.h> 
int main()
{
//char* surnames = //{"Hogan","Rose","Burns","Park","Burris","Garza","Farrell","Phillips","Cline","Bentley","Thornton","Beard","Patton","Thornton","Scott","Vaughan","Castillo","Duffy","Gomez","Steele","Stanley","Miranda","Mcpherson","Chase","Cox","Joyner","Bryant","Peck","Mathews","Shelton","Bauer","Hoover","Vance","Hensley","Torres","Cole","Petty","Zamora","Anthony","Andrews","Tucker","Neal","Avery","Garrett","Kinney","Holmes","Rich","Blanchard","Flores","Mckenzie","Klein","Wynn","Lott","Harmon","Harper","Wolfe","Knox","Dickson","Hardin","Davenport","King","Reeves","Cruz","Porter","Woods","Estrada","Garrison","Bennett","Russell","Howe","Aguirre","Reilly","English","Santiago","Caldwell","Pena","Jackson","Black","Hawkins","Walker","Lopez","Yates","Rivera","Stout","Thomas","Holloway","Sparks","Hester","Ayala","Olson","Joyner","Donovan","Robinson","Lucas","Rosario","Fernandez","Castillo","Hodges","Middleton","Davenport","Dotson","Berry","Sykes","Becker","Solis","Franklin","Gould","Robertson","Cleveland","Velasquez","Berg","Pearson","Holland","Skinner","Stein","Everett","Hensley","Henson","Wolf","Burton","Sawyer","Odom","Vance","Porter","Martin","Randall","Campbell","Warner","Benjamin","Lancaster","Carpenter","Simmons","Sampson","Serrano","Gentry","Harris","Mcfarland","Carlson","Berry","Robbins","Noel","Middleton","Rosa","Mills","Ryan","Good","Rodriguez","Marsh","Beasley","Harris","Strong","Small","Richards","Valencia","Yang","Shields","Cote","Acevedo","Klein","Booker","York","Simon","Vazquez","Cross","Short","Herrera","Drake","Ballard","Clements","Mitchell","Randolph","Lamb","Carlson","Gillespie","Berry","Sparks","Morrison","Roberts","Richards","Simmons","Gray","Branch","Flynn","Knight","Robles","Waters","Leach","Hinton","Reeves","Brady","Nielsen","Moore","Snider","Odonnell","Caldwell","Haley","Bender","Church","Beach","Waters","Dotson","Berry","Sykes","Becker","Solis","Franklin","Gould","Robertson","Cleveland","Velasquez","Berg","Pearson","Holland","Skinner","Stein","Everett","Hensley","Henson","Wolf","Burton","Sawyer","Odom","Vance","Porter","Martin","Randall","Campbell","Warner","Benjamin","Lancaster","Carpenter","Simmons","Sampson","Serrano","Gentry","Harris","Mcfarland","Carlson","Berry","Robbins","Noel","Middleton","Rosa","Mills","Ryan","Good","Rodriguez","Marsh","Beasley","Harris","Strong","Small","Richards","Valencia","Yang","Shields","Cote","Acevedo","Klein","Booker","York","Simon","Vazquez","Cross","Short","Herrera","Drake","Ballard","Clements","Mitchell","Randolph","Lamb","Carlson","Gillespie","Berry","Sparks","Morrison","Roberts","Richards","Simmons","Gray","Branch","Flynn","Knight","Robles","Waters","Leach","Hinton","Reeves","Brady","Nielsen","Moore","Snider","Odonnell","Caldwell","Haley","Bender","Church","Beach","Waters","Jordan","Ratliff","Delacruz","Vaughn","Mccarty","Flores","Hernandez","Willis","Beach","Hancock","Kane","Petty","Bradford","Dyer","Estes","Mendez","Zimmerman","Mooney","Mcdaniel","Hanson","Hester","Koch","Kane","Sanford","Orr","Howell","Sandoval","Slater","Mendoza","Buckley","Campbell","Baxter","Black","Chaney","Bullock","Higgins","Becker","Fleming","Wynn","Greene","Richard","Estes","Aguilar","Bauer","Andrews","Raymond","Allen","Wagner","Montgomery","Bentley","Molina","Santos","Cain","Mooney","Sweeney","Santiago","Mcmahon","Wong","Garrett","Maxwell","Blair","Park","Hudson","Patterson","Erickson","Burnett","Walsh","Pollard","Warren","Mccormick","Jefferson","Obrien","Salazar","Gross","Mooney","Howell","Butler","Bowman","Bowen","Castro","Vang","Chan","Albert","Weeks","Juarez","Wallace","Vasquez","Contreras","Ward","Heath","Myers","Todd","Reilly","Crosby","Ramirez","Watts","Gross","Briggs","Perry","Green","Morris","Goodman","Faulkner","Ryan","Wright","Ratliff","Gross","Rose","Franks","Whitehead","Sanders","Weber","Mendez","Frederick","Kramer","Slater","Wooten","Wells","Pierce","Rush","Dickerson","Graves","Rivers","Ballard","Holloway","Sweeney","Nielsen","Barron","Burns","David","Walsh","Fox","Bean","King","West","West","Ross","Alston","Holland","Downs","Whitehead","Marks","Adkins","Newton","Lyons","Blackburn","Heath","Juarez","Tillman","Harrington","Mathis","Richards","Maldonado","Santiago","Thomas","Tyson","Frye","Jacobs","Francis","Owen","Hodges","French","Hopper","Herring","George","Fleming","Burke","Zamora","Coffey","Hoffman","Fisher","Powers","Stewart","Rowe","Ramirez","Vaughan","Mullins","Horne","Horn","England","Walton","Conrad","Crosby","Ford","Tran","Mcintyre","Elliott","Glass","Morse","Everett","Dean","Mcdowell","Duffy","Velazquez","Whitley","Mcdonald","Reilly","Coleman","Carter","Compton","Workman","Bailey","Gates","Farrell","Higgins","Dawson","Nielsen","Benson","Hendricks","Morrison","Bauer","Scott","White","Barnes","Reed","Lloyd","Cantu","Parks","Bird","Macias","Vaughan","Myers","Robbins","Hickman","Aguirre","Rodriquez","Hubbard","Gamble","French","Sullivan","Lang","Wiggins","Blackburn","Dominguez","Larsen","Delacruz","Hinton","Abbott","Stanley","Ryan","Romero","Kidd","Cobb","Waters","Saunders","Serrano","Castillo","Bentley","Hale","Salas","Trujillo","Bell","Livingston","Petersen","Brooks","Mcdaniel","Reyes","Waters","Lancaster","Mitchell","Wilder","Mullins","Daniel","Mcgowan","Murray","Berger","David","Forbes","Chang","Horne","Christian","Fleming","Barry","Barrett","Burke","Smith","Wong","Coleman","Porter","Estes","Koch","York","Beard","Hood","Horne","Brennan","Gilbert","Clayton","Cantu","Walton","Huff","Thompson","Huber","Poole","Riggs","Fitzgerald","Peck","Johnson","Mcclain","Battle","Leblanc","Vaughan","Monroe","Huber","Collier","Guthrie","Lucas","Burton","Alston","Barlow","Dudley","Marquez","Vance","Huffman","Hendrix","Carney","Harmon","Horton","Hicks","Rivera","Walls","Donovan","Franklin","Case","Harper","Wyatt","Rhodes","Holman","Dodson","Dejesus","Vargas","Buckner","Alvarado","Evans","Contreras","Stephenson","Case","Brady","Johns","Hahn","Andrews","Mack","Crawford","Moreno","Hicks","Bowen","Henry","Noble","Hale","Roman","Norman","Cotton","Hartman","Turner","Adams","Wiggins","Burns","Collier","Meyer","Payne","Bright","Cote","Key","Travis","Daniel","Michael","Hancock","Kirby","Dorsey","Luna","Vincent","Barlow","Haney","Grant","Love","Marshall","Horton","Crane","Campbell","Rose","Bradley","Baird","Bryan","Mayer","Chaney","Mathis","Gates","Williams","Owen","Raymond","Snyder","Gallagher","Branch","Ruiz","Beach","Hutchinson","Camacho","Dominguez","Gray","Wiggins","Kirkland","Brady","Riddle","Carrillo","Contreras","Gallegos","Mathews","Robertson","Nicholson","Clarke","Whitfield","Cohen","Porter","Rice","Mendez","Porter","Mcmahon","Prince","Witt","Mcconnell","Barber","Buckner","Benjamin","Hogan","Strong","Tyler","Nolan","Berry","Sweet","Watts","Ayers","Moon","Delacruz","Graham","Wilkerson","Oconnor","Craig","Hughes","Stuart","Sloan","Good","Byers","Chen","Hood","Fleming","Henry","Sweeney","Adams","Stafford","Terrell","Mcintyre","Michael","Hebert","Wise","Bowman","Gomez","Howe","Burton","Barr","Kline","Houston","Espinoza","Garcia","Hughes","Mueller","Allen","Cole","Cooke","Love","Carrillo","Glover","Nixon","Savage","Robinson","Little","Bowers","Gill","Duke","Griffin","Hicks","Mann","Berg","Knox","Reeves","Burgess","Melton","Blevins","Fletcher","Prince","Small","Blankenship","Carpenter","Snow","Barnett","Morales","Petty","William","Dickerson","Conley","Christensen","Mckinney","Dotson","Maldonado","Ortiz","Byers","Munoz","Cantu","Ware","Mason","Martin","Sullivan","Carson","Randolph","Hansen","Hyde","Tran","Sargent","Vance","Foley","Drake","Hayes","Payne","Vasquez","Eaton","Little","Chase","Chan","Haynes","Mcmillan","Duncan","Cruz","Mayo","Boyle","Moss","Burnett","Mueller","Mercado","Frazier","Chan","Walker","Owen","Norton","Rush","Deleon","Oneil","Rivas","Rios","Blackburn","Conrad","Joseph","Sanford","Nash","Ward","Benjamin","Downs","Ewing","Schmidt","Larson","Cruz","Gonzales","Black","Mueller","Hess","Bridges","Blanchard","Weaver","Barrett","Reynolds","Kelly","Ramsey","Flores","Herrera","Rhodes","Sweet","Booker","Wood","Hunt","Jimenez","Torres","Mckinney","Gamble","Ballard","Ellison","Byers","Kemp","Wagner","Vazquez","Gallagher","Reed","Francis","Avila","Strong","Park","Wiggins","Peters","Whitehead","Michael","Jarvis","Merrill","Wong","Mullins","Meyer","Barr","Joyner","Mcguire","Sawyer","Woodward","Santiago","Dixon","Rice","Walter","Baldwin","Larsen","Estes","Erickson","Glover","Combs","Schneider","Washington","Brooks","Cochran","Wilkerson","Lawrence","Ferguson","Fletcher","Crawford","Stokes","Mccall","Carrillo","Gonzales","Horton","Patterson","Downs","Whitley","Sellers","Oneill","Blake","Goodman","Swanson","Huff","Welch","Baldwin","Pugh","Smith","Stephens","Sanders","Chase","Bailey","Wilkerson","Branch","Oneil","Farmer","Barr","Savage","Calderon","Obrien","Ashley","Estes","Massey","Lopez","Maxwell","Woodward","Avery","Finch","Gutierrez","Allison","Delgado","Fulton","Huff","Copeland","Wiley","Keith","Reid","Marshall","Maynard","Copeland","Middleton","Wagner","Mullen","Yang","Beasley","Bird","Bowen","Mays","Olson","Best","Ewing","Taylor","Horton","Tanner","Alvarado","Terry","Pickett","Matthews","Aguilar","Vang","Daugherty","Prince","Abbott","Hernandez"};
    

	//for list
    struct tms start;
    times(&start);
    clock_t real_start = clock();

    struct tms t;
    clock_t c;

    List* l= initList();
  
    for(int i=0;i<1000;i++)
    {
	
	AddPerson(l,i,"N");
    times(&t);
    c=clock();
    printf("START: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );

	}

    times(&t);
    c=clock();
    printf("\nTime of making adress book on List:\n");	
    printf("START: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );

	
    
//time measure of delete operation
    times(&start);
    real_start = clock();	
    deletePerson(l,50,"N");
    times(&t);
    c=clock();

    printf("Delete operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );

//time measure of search operation
    times(&start);
    real_start = clock();
    findPerson(l,100,"N");
    times(&t);
    c=clock();

    printf("Find operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
    

//time measure of sorting
     times(&start);
     real_start = clock();
     sortListByNumber(l,4);     
     times(&t);
     c=clock();

    printf("Sort operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
	
	//for tree
	printf("\nFor tree structure: \n");
    times(&start);
    real_start = clock();


    Tree* tree= initTree();

    for(int i=0;i<1000;i++)
    {
	
        AddPersonT(tree,tree->root,i,"M");
        times(&t);
        c=clock();
        printf("START: real: %.10f, user: %.10f, sys: %.10f\n", \
        (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
        (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
        (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
	}

    times(&t);
    c=clock();
    printf("\nTime of making adress book on Tree:\n");	
    printf("START: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
	
    
//time measure of delete operation
    times(&start);
    real_start = clock();
	PersonT* per = malloc(sizeof(PersonT));
	per->a=10;
	strcpy(per->name,"M");
    deletePersonT(tree,per);
    times(&t);
    c=clock();

    printf("Delete operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );

//time measure of search operation
    times(&start);
    real_start = clock();
    findPersonT(tree,tree->root,100,"M");
    times(&t);
    c=clock();

    printf("Find operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );
    

//time measure of sorting
     times(&start);
     real_start = clock();
     sortedTree(tree);     
     times(&t);
     c=clock();

    printf("Sort operation: real: %.10f, user: %.10f, sys: %.10f\n", \
    (double)(c-real_start)/(double)CLOCKS_PER_SEC,\
    (double)(t.tms_utime - start.tms_utime)/(double)sysconf(_SC_CLK_TCK),\
    (double)(t.tms_stime - start.tms_stime)/(double)sysconf(_SC_CLK_TCK) );


    return 0;
}
