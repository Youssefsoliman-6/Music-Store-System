#include <iostream> 
#include <stdlib.h> //included for clsfunction
#include <cctype>   //included for isspace and ispunct functions in the capitalizing function
#include <fstream>  //included for file handling
#include <string>   //included for getline function
#include <iomanip>  //included for setw() function
using namespace std;

//function to capitalize first letter of every word in string to optimize user input to match existing content when searching
string capitalizeFirst(const string& original) {
    string result = original;
    bool capitalizeNext = true;

    for (size_t i = 0; i < result.size(); ++i) {
        if (isspace(result[i]) || ispunct(result[i])) {
            capitalizeNext = true;
        }
        else if (capitalizeNext && isalpha(result[i])) {
            result[i] = toupper(result[i]);
            capitalizeNext = false;
        }
        else {
            result[i] = tolower(result[i]);
        }
    }

    return result;
}


// Enum for genre
enum MusicGenre {
    Rock,
    Pop,
    Progressive,
    HipHop,
    Metal
};

//Album structure
struct Album {
    string Name;
    string ArtistName;
    MusicGenre Genre;
    double Price = 0.0; // Initialize to avoid warning

};

//Adress struct
struct Address {
    string Governorate;
    string DistrictName;
    int StreetNum = 0; // Initialize to avoid warning
    int BuildingNum = 0; // Initialize to avoid warning
    int ApartmentNum = 0; // Initialize to avoid warning
};

//Order struct
struct Order {
    int numAlbumsInCart = 0;
    int orderID = 0; // Initialize to avoid warning
    int customerID = 0; // Initialize to avoid warning
    double totalAmount = 0.0; // Initialize to avoid warning
    Album albumsOrdered[20];
    Address deliveryAddress;
};

//Function prototypes
string genreToString(const MusicGenre genre);
MusicGenre stringToGenre(const string& genreString);
void readAlbumsFromFile(const string& filename, Album albums[], int Max, int& albumCount);
void writeAlbumsToFile(const string& filename, const Album albums[], int albumCount);
void addToCart(Order& order, const Album album);
void removeFromCart(Order& order, const Album album);
void displayAlbums(Album albums[], int albumCount);
double calculateTotal(const Order& order);
bool LogIn(string& user, string& password);
void displayMenu();
void addNewAlbum(Album albums[], int& numAlbums);
void displayCart(Order order);
void displayAlbumsByGenre(Album albums[], int numAlbums);
void displayAlbumsByArtist(const Album albums[], int numAlbums);
Order fillAddress(Order& order);

//Global variables to be used through any function
const int MAX_ALBUMS = 100;
int numAlbums = 55;
Order order1;
Album albums[MAX_ALBUMS]; //array of all album structs
Album albumsOrdered[20]; //array for albums ordered 

//function to take address info from user
Order fillAddress(Order& order) {
    Address address;
    int choice;
    string governorate, districtName;
    int streetNum, buildingNum, apartmentNum;
    cout << "Enter Govornorate name: " << endl;
    cin >> governorate;
    cout << "Enter District name: " << endl;
    cin >> districtName;
    cout << "Enter Street number: " << endl;
    cin >> streetNum;
    cout << "Enter Building number: " << endl;
    cin >> buildingNum;
    cout << "Enter Apartment number: " << endl;
    cin >> apartmentNum;
    cout << "Enter User ID: " << endl;
    cin >> order.customerID;
    cout << "Enter Order ID: " << endl;
    cin >> order.orderID;
    address.Governorate = governorate;
    address.DistrictName = districtName;
    address.StreetNum = streetNum;
    address.BuildingNum = buildingNum;
    address.ApartmentNum = apartmentNum;
    cout << "Confirm order?:" << endl << "1.Yes" << endl << "2.No" << endl;
    cin >> choice;
    if (choice == 1) {
        cout << "Order Confirmed." << endl;
        displayMenu();
    }
    else if (choice == 2) {
        cout << "Back to Main Menu." << endl;
        displayMenu();
    }
    return order; 
}

// Function to convert Genre enum to string
string genreToString(const MusicGenre genre) {
    switch (genre) {
    case Rock: return "Rock";
    case Pop: return "Pop";
    case Progressive: return "Progressive";
    case HipHop: return "Hip Hop";
    case Metal: return "Metal";
    default: return "Unknown";
    }
}

//function to convert string to Genre enum
MusicGenre stringToGenre(const string& genreString) {
    if (genreString == "Rock") return Rock;
    else if (genreString == "Pop") return Pop;
    else if (genreString == "Progressive") return Progressive;
    else if (genreString == "Hip Hop") return HipHop;
    else  if (genreString == "Metal") return Metal;
    else {
        cout << "Invalid Genre. Please Choose from The displayed genres.";
        return Rock;
    }
}

//Function to read album info from file and store it an array of album structs
void readAlbumsFromFile(const string& filename, Album albums[], int Max, int& albumCount) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    string line;
    albumCount = 0;
    while (getline(file, line) && albumCount < Max) {
        //Extract fields based on fixed-width columns
        string albumName = line.substr(0, 40);
        string artistName = line.substr(40, 25);
        string genreStr = line.substr(65, 12);
        string priceStr = line.substr(77, 5);

        //Trim leading and trailing spaces
        albumName.erase(0, albumName.find_first_not_of(' '));
        albumName.erase(albumName.find_last_not_of(' ') + 1);
        artistName.erase(0, artistName.find_first_not_of(' '));
        artistName.erase(artistName.find_last_not_of(' ') + 1);
        genreStr.erase(0, genreStr.find_first_not_of(' '));
        genreStr.erase(genreStr.find_last_not_of(' ') + 1);
        priceStr.erase(0, priceStr.find_first_not_of(' '));
        priceStr.erase(priceStr.find_last_not_of(' ') + 1);

        //Store attributes in array of structs
        albums[albumCount].Name = albumName;
        albums[albumCount].ArtistName = artistName;
        //Convert genre string to enum
        if (genreStr == "Rock") albums[albumCount].Genre = Rock;
        else if (genreStr == "Pop") albums[albumCount].Genre = Pop;
        else if (genreStr == "Progressive") albums[albumCount].Genre = Progressive;
        else if (genreStr == "Hip Hop") albums[albumCount].Genre = HipHop;
        else if (genreStr == "Metal") albums[albumCount].Genre = Metal;
        else albums[albumCount].Genre = Rock; // Default to Rock if unknown
        albums[albumCount].Price = stoi(priceStr);

        albumCount++;
    }

    file.close();
}

//Function to write albums from array of structs to file
void writeAlbumsToFile(const string& filename, const Album albums[], int albumCount) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    for (int i = 0; i < albumCount; ++i) {
        file << setw(40) << left << albums[i].Name
            << setw(25) << left << albums[i].ArtistName
            << setw(12) << left << genreToString(albums[i].Genre)
            << setw(5) << right << albums[i].Price << endl;
    }
    file.close();
}

//Function to add album to cart
void addToCart(Order& order, const Album album) {
    if (order.numAlbumsInCart < 20) { // Check if there's space in the cart
        // Check if the album is already in the cart
        for (int j = 0; j < order.numAlbumsInCart; ++j) {
            if (order.albumsOrdered[j].Name == album.Name) {
                cout << "Album '" << album.Name << "' is already in the cart." << endl;
                return;
            }
        }
        // Add the album to the cart
        order.albumsOrdered[order.numAlbumsInCart] = album;
        ++order.numAlbumsInCart; // Increment the count of albums in the cart
        cout << "Album '" << album.Name << "' added to the cart." << endl;
    }
    else {
        cout << "Cart is full. Cannot add album." << endl;
    }
}

//Function to remove album from cart
void removeFromCart(Order& order, const Album album) {
    bool found = false;
    for (int i = 0; i < 20; i++) { // Start from 0 index
        if (order.albumsOrdered[i].Name == album.Name) {
            order.albumsOrdered[i] = Album();   // Set to default constructed Album
            cout << "Album: " << album.Name << " removed from cart" << endl;
            found = true;
            break;
        }
    }
   if(!found) 
       cout << "Album: " << album.Name << " not found in cart" << endl; // Moved this line from inside the loop
}

//Function to display albums
void displayAlbums(Album albums[], int albumCount) {
    int choice;
    for (int i = 0; i < albumCount; ++i) {
        cout << i + 1 << ". ";
        cout << "Album Name: " << setw(40) << left << albums[i].Name;
        cout << "Artist Name: " << setw(25) << left << albums[i].ArtistName;
        cout << "Genre: " << setw(12) << left << genreToString(albums[i].Genre);
        cout << "Price: " << albums[i].Price << endl;
    }
    cout << "1. Add Album To Cart." << endl;
    cout << "2. Remove Album from Cart." << endl;
    cout << "3. Go to Main Menu." << endl;
    cout << "Enter Your Choice: " << endl;
    cin >> choice;

    if (choice == 1) {
        int albumnum;
        //displayAlbums(albums, MAX_ALBUMS);
        cout << "Enter Album Number to Add to Cart: " << endl;
        cin >> albumnum;
        addToCart(order1, albums[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 2) {
        int albumnum;
        displayCart(order1);
        cout << "Enter Album Number to Remove from Cart: " << endl;
        cin >> albumnum;
        removeFromCart(order1, order1.albumsOrdered[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 3) {
        system("cls");
        displayMenu();
    }
    else
        cout << "invalid Choice. Please Choose from The Displayed Numbers." << endl;
}

//Function to calculate total price of order
double calculateTotal(const Order& order) {
    double Total = 0.0;
    for (int i = 0; i < 20; i++) { // Start from 0 index
        if (!order.albumsOrdered[i].Name.empty()) {
            Total += order.albumsOrdered[i].Price;
        }
    }
    return Total;
}

//Function to authenticate login info by comparing it to a deafault username and password set for easy testing
bool LogIn(string& user, string& password) {
    string storedName = "user";
    string storedPassword = "password123";

    return (user == storedName && password == storedPassword);
}

//function to display the main menu
void displayMenu() {
    int choice;
    cout << "Menu:" << endl;
    cout << "1. Display Albums" << endl;
    cout << "2. Display available albums by Genre" << endl;
    cout << "3. Display available albums by Artist" << endl;
    cout << "4. Add a new Music Album" << endl;
    cout << "5. Display Cart" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice) {
    case 1:
        displayAlbums(albums, MAX_ALBUMS);
        break;
    case 2:
        displayAlbumsByGenre(albums, MAX_ALBUMS);
        break;
    case 3:
        displayAlbumsByArtist(albums, MAX_ALBUMS);
        break;
    case 4:
        addNewAlbum(albums, numAlbums);
        break;
    case 5:
        displayCart(order1);
        break;
    case 6:
        cout << "Exiting Program.";
        return;
    default:
        cout << "Invalid input. Please Enter a Number Between 1-6.";
        displayMenu();
    }


}

//Function to add a new album to array and file
void addNewAlbum(Album albums[], int& numAlbums) {
    if (numAlbums < 100) {
        Album newAlbum;
        string genrestr;
        cout << "Enter Album Name: " << endl;
        cin >> newAlbum.Name;
        cout << "Enter Artist Name: " << endl;
        cin >> newAlbum.ArtistName;
        cout << "Enter Genre: " << endl;
        cin >> genrestr;
        capitalizeFirst(genrestr);
        newAlbum.Genre = stringToGenre(genrestr);
        cout << "Enter Price: " << endl;
        cin >> newAlbum.Price;
        albums[numAlbums++] = newAlbum;
        cout << "New Album added successfully." << endl;
        writeAlbumsToFile("AlbumInfo.txt", albums, MAX_ALBUMS);
        displayMenu();
    }
    else {
        cerr << "Maximum number of albums reached. " << endl;
        displayMenu();
    }
}

//Function to display cart
void displayCart(Order order) {
    int choice;
    for (int i = 0; i < 20; i++) {
        cout << i + 1 << ". ";
        cout << "Album Name: " << setw(40) << left << order.albumsOrdered[i].Name;
        cout << "Artist Name: " << setw(25) << left << order.albumsOrdered[i].ArtistName;
        cout << "Genre: " << setw(12) << left << genreToString(order.albumsOrdered[i].Genre);
        cout << "Price: " << order.albumsOrdered[i].Price << endl;
    }
    cout << "1. Add Album To Cart." << endl;
    cout << "2. Remove Album from Cart." << endl;
    cout << "3. Go to Main Menu." << endl;
    cout << "4. Confirm Order." << endl;
    cout << "Enter Your Choice: " << endl;
    cin >> choice;

    if (choice == 1) {
        int albumnum;
        displayAlbums(albums, MAX_ALBUMS);
        cout << "Enter Album Number to Add to Cart: " << endl;
        cin >> albumnum;
        addToCart(order1, albums[albumnum - 1]);
        cout << "Album Added to Cart." << endl;
        displayMenu();
    }
    else if (choice == 2) {
        int albumnum;
        displayCart(order1);
        cout << "Enter Album Number to Remove from Cart: " << endl;
        cin >> albumnum;
        removeFromCart(order1, order1.albumsOrdered[albumnum - 1]);
        cout << "Album Removed from Cart." << endl;
        displayMenu();
    }
    else if (choice == 3) {
        system("cls");
        displayMenu();
    }
    else if (choice == 4) {
        fillAddress(order1);
    }
    else
        cout << "invalid Choice. Please Choose from The Displayed Numbers." << endl;
}

//Function to display albums by genre
void displayAlbumsByGenre(Album albums[], int numAlbums){
    int choice;
    cout << "Choose a Genre: " << endl;
    cout << "1.Rock" << endl << "2.Pop" << endl << "3.progressive" << endl << "4.Hip Hop" << endl << "5.Metal" << endl << "6.Display All Albums" << endl;
    cin >> choice;

    for (int i = 0; i < numAlbums; i++) {
        if (albums[i].Genre == choice - 1) {
            cout << i + 1 << ". ";
            cout << "Album Name: " << setw(40) << left << albums[i].Name;
            cout << "Artist Name: " << setw(25) << left << albums[i].ArtistName;
            cout << "Genre: " << setw(12) << left << genreToString(albums[i].Genre);
            cout << "Price: " << albums[i].Price << endl;
        }
        //else continue;
    }
    cout << "1. Add Album To Cart." << endl;
    cout << "2. Remove Album from Cart." << endl;
    cout << "3. Go to Main Menu." << endl;
    cout << "Enter Your Choice: " << endl;
    cin >> choice;

    if (choice == 1) {
        int albumnum;
        cout << "Enter Album Number to Add to Cart: " << endl;
        cin >> albumnum;
        addToCart(order1, albums[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 2) {
        int albumnum;
        displayCart(order1);
        cout << "Enter Album Number to Remove from Cart: " << endl;
        cin >> albumnum;
        removeFromCart(order1, order1.albumsOrdered[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 3) {
        system("cls");
        displayMenu();
    }
    else
        cout << "invalid Choice. Please Choose from The Displayed Numbers." << endl;
}

//Function to display albums by artist
void displayAlbumsByArtist(const Album albums[], int numAlbums) {
    bool found = false;
    int choice;
    string artistName;
    cout << "Enter Artist Name: " << endl;
    getline(cin, artistName);
    cout << "Available albums by " << capitalizeFirst(artistName) << ": " << endl;
    for (int i = 0; i < numAlbums; i++) {
        if (albums[i].ArtistName == artistName) {
            cout << "Album Name: " << setw(40) << left << albums[i].Name;
            cout << "Artist Name: " << setw(25) << left << albums[i].ArtistName;
            cout << "Genre: " << setw(12) << left << genreToString(albums[i].Genre);
            cout << "Price: " << albums[i].Price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No albums found for artist " << artistName << endl;
    }
    cout << "1. Add Album To Cart." << endl;
    cout << "2. Remove Album from Cart." << endl;
    cout << "3. Go to Main Menu." << endl;
    cout << "Enter Your Choice: " << endl;
    cin >> choice;

    if (choice == 1) {
        int albumnum;
        cout << "Enter Album Number to Add to Cart: " << endl;
        cin >> albumnum;
        addToCart(order1, albums[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 2) {
        int albumnum;
        displayCart(order1);
        cout << "Enter Album Number to Remove from Cart: " << endl;
        cin >> albumnum;
        removeFromCart(order1, order1.albumsOrdered[albumnum - 1]);
        displayMenu();
    }
    else if (choice == 3) {
        system("cls");
        displayMenu();
    }
    else
        cout << "invalid Choice. Please Choose from The Displayed Numbers." << endl;
}


int main() {

    readAlbumsFromFile("C:\\1RO\\UNI ;(\\Stuff\\Group 45. Music Store System Final Project\\Music Store System Code\\ConsoleApplication13\\AlbumInfo.txt", albums, MAX_ALBUMS, numAlbums);

    string name, password;
    cout << "Welcome to The Music Store!" << endl;
    //prompt user to enter credentials
    do  //Do-While loop to keep propmting user to enter again if credentials are incorrect
    {
        cout << "Please Enter Your LogIn Information Below: " << endl;
        cout << "Enter username: " << endl;
        cin >> name;
        cout << "Enter Password: " << endl;
        cin >> password;
        //Check validity of login info
        if (LogIn(name, password)) {
            cout << "LogIn Successful." << endl << "Welcome " << name << " !" << endl;
            displayMenu();
        }
        else {
            cout << "Invalid Username or Password " << endl << "Please Try Again." << endl;
        }
    } while (!LogIn(name, password));
}