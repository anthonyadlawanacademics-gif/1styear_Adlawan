#include <iostream>
#include <string>
#include <vector>

using namespace std;

void showSlide(const string& title, const vector<string>& points) {
    cout << "\n==================================================\n";
    cout << "        " << title << "\n";
    cout << "==================================================\n\n";

    for (const auto& point : points) {
        cout << "- " << point << "\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    cout << "\n";
}

int main() {
    cout << "Welcome to the presentation!\n";
    cout << "Press Enter to start...";
    cin.ignore();
    cin.get();

    showSlide("Introduction", {
        "This is a simple presentation in C++.",
        "It shows how to create slides in the console.",
        "You can customize the content easily."
    });

    showSlide("Agenda", {
        "1. Overview",
        "2. Key Features",
        "3. Summary"
    });

    showSlide("Key Features", {
        "Easy to understand and edit.",
        "Works in any terminal or console.",
        "Great for quick demonstrations."
    });

    showSlide("Summary", {
        "Simple, clean, and effective.",
        "Perfect for short presentations.",
        "Thanks for watching!"
    });

    cout << "End of presentation.\n";
    return 0;
}
