#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFrame>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iostream>

using namespace std;

// ==========================================
// 1. THE GAME LOGIC (The Brain)
// ==========================================

enum Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };

class Card {
public:
    Rank rank;
    Suit suit;
    Card(Rank r, Suit s) : rank(r), suit(s) {}

    int getValue() const {
        if (rank >= JACK && rank <= KING) return 10;
        if (rank == ACE) return 11;
        return static_cast<int>(rank);
    }

    // Returns a string for the GUI to display (e.g., "A♥")
    QString displayString() const {
        const char* ranks[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
        // Using standard ASCII for simplicity, or unicode symbols
        const char* suits[] = { "\u2663", "\u2666", "\u2665", "\u2660" }; // ♣ ♦ ♥ ♠
        return QString("%1%2").arg(ranks[rank - 2]).arg(suits[suit]);
    }

    // Helper to color hearts and diamonds red
    bool isRed() const { return suit == DIAMONDS || suit == HEARTS; }
};

class Deck {
private:
    vector<Card> cards;
public:
    Deck() {
        srand(time(NULL));
        populate();
    }

    void populate() {
        cards.clear();
        for (int s = CLUBS; s <= SPADES; ++s) {
            for (int r = TWO; r <= ACE; ++r) {
                cards.emplace_back(static_cast<Rank>(r), static_cast<Suit>(s));
            }
        }
        shuffleDeck();
    }

    void shuffleDeck() {
        for (size_t i = 0; i < cards.size(); ++i) {
            int r = rand() % cards.size();
            swap(cards[i], cards[r]);
        }
    }

    Card deal() {
        if (cards.empty()) populate();
        Card c = cards.back();
        cards.pop_back();
        return c;
    }
};

class Hand {
public:
    vector<Card> cards;

    void addCard(Card c) { cards.push_back(c); }
    void clear() { cards.clear(); }

    int getTotal() const {
        int total = 0;
        int aceCount = 0;
        for (const auto& card : cards) {
            int val = card.getValue();
            total += val;
            if (card.rank == ACE) aceCount++;
        }
        while (total > 21 && aceCount > 0) {
            total -= 10;
            aceCount--;
        }
        return total;
    }
};

// ==========================================
// 2. THE QT GUI
// ==========================================

class BlackjackWindow : public QWidget {
private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    int balance;
    int currentBet;
    bool gameInProgress;

    // UI Components
    QLabel* dealerLabel;
    QHBoxLayout* dealerCardsLayout;
    QLabel* dealerScoreLabel;

    QLabel* playerLabel;
    QHBoxLayout* playerCardsLayout;
    QLabel* playerScoreLabel;

    QLabel* statusLabel;
    QLabel* balanceLabel;

    QPushButton* hitButton;
    QPushButton* standButton;
    QPushButton* dealButton;

public:
    BlackjackWindow(QWidget *parent = nullptr) : QWidget(parent), balance(1000), gameInProgress(false) {
        setupUI();
        resetGame();
    }

    // --- UI SETUP ---
    void setupUI() {
        setWindowTitle("Qt C++ Blackjack");
        resize(600, 500);

        // Main Layout
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(20);

        // Style: Green felt background
        this->setStyleSheet("background-color: #2E8B57; font-family: Arial;");

        // --- Top: Dealer Area ---
        dealerLabel = new QLabel("Dealer's Hand", this);
        dealerLabel->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
        mainLayout->addWidget(dealerLabel);

        dealerCardsLayout = new QHBoxLayout();
        dealerCardsLayout->setAlignment(Qt::AlignLeft);
        mainLayout->addLayout(dealerCardsLayout);

        dealerScoreLabel = new QLabel("Score: ?", this);
        dealerScoreLabel->setStyleSheet("color: white; font-size: 14px;");
        mainLayout->addWidget(dealerScoreLabel);

        // --- Middle: Status ---
        statusLabel = new QLabel("Place your bet to start!", this);
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet("color: yellow; font-size: 18px; font-weight: bold; background-color: rgba(0,0,0,0.3); padding: 10px; border-radius: 5px;");
        mainLayout->addWidget(statusLabel);

        // --- Bottom: Player Area ---
        playerLabel = new QLabel("Your Hand", this);
        playerLabel->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
        mainLayout->addWidget(playerLabel);

        playerCardsLayout = new QHBoxLayout();
        playerCardsLayout->setAlignment(Qt::AlignLeft);
        mainLayout->addLayout(playerCardsLayout);

        playerScoreLabel = new QLabel("Score: 0", this);
        playerScoreLabel->setStyleSheet("color: white; font-size: 14px;");
        mainLayout->addWidget(playerScoreLabel);

        // --- Controls ---
        balanceLabel = new QLabel("Balance: $1000", this);
        balanceLabel->setStyleSheet("color: white; font-size: 14px;");
        mainLayout->addWidget(balanceLabel);

        QHBoxLayout* buttonsLayout = new QHBoxLayout();

        dealButton = new QPushButton("Deal ($100)", this);
        hitButton = new QPushButton("Hit", this);
        standButton = new QPushButton("Stand", this);

        // Button Styling
        QString btnStyle = "QPushButton { background-color: #f0f0f0; border-radius: 5px; padding: 10px; font-weight: bold; }"
                           "QPushButton:hover { background-color: #e0e0e0; }"
                           "QPushButton:disabled { background-color: #a0a0a0; color: #505050; }";
        dealButton->setStyleSheet(btnStyle);
        hitButton->setStyleSheet(btnStyle);
        standButton->setStyleSheet(btnStyle);

        buttonsLayout->addWidget(dealButton);
        buttonsLayout->addWidget(hitButton);
        buttonsLayout->addWidget(standButton);
        mainLayout->addLayout(buttonsLayout);

        // Connect Signals to Slots (Lambda functions for logic)
        connect(dealButton, &QPushButton::clicked, this, &BlackjackWindow::startRound);
        connect(hitButton, &QPushButton::clicked, this, &BlackjackWindow::onHit);
        connect(standButton, &QPushButton::clicked, this, &BlackjackWindow::onStand);
    }


    void clearLayout(QLayout* layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    // Creates a visual card widget
    QLabel* createCardWidget(const Card& card) {
        QLabel* lbl = new QLabel(card.displayString());
        lbl->setFixedSize(70, 100);
        lbl->setAlignment(Qt::AlignCenter);

        QString color = card.isRed() ? "red" : "black";
        // CSS to make the Label look like a Card
        lbl->setStyleSheet(QString("background-color: white; color: %1; border: 1px solid black; border-radius: 5px; font-size: 20px; font-weight: bold;").arg(color));
        return lbl;
    }

    QLabel* createBackCardWidget() {
        QLabel* lbl = new QLabel("?");
        lbl->setFixedSize(70, 100);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("background-color: #8B0000; color: white; border: 2px solid white; border-radius: 5px; font-size: 24px;");
        return lbl;
    }

    void updateUI(bool revealDealer) {
        // Update Player UI
        clearLayout(playerCardsLayout);
        for(const auto& c : playerHand.cards) {
            playerCardsLayout->addWidget(createCardWidget(c));
        }
        playerScoreLabel->setText("Score: " + QString::number(playerHand.getTotal()));

        // Update Dealer UI
        clearLayout(dealerCardsLayout);
        if (dealerHand.cards.empty()) return;

        if (!revealDealer) {
            // Show first card, hide second
            dealerCardsLayout->addWidget(createCardWidget(dealerHand.cards[0]));
            dealerCardsLayout->addWidget(createBackCardWidget()); // Hole card
            dealerScoreLabel->setText("Score: ?");
        } else {
            // Show all
            for(const auto& c : dealerHand.cards) {
                dealerCardsLayout->addWidget(createCardWidget(c));
            }
            dealerScoreLabel->setText("Score: " + QString::number(dealerHand.getTotal()));
        }

        balanceLabel->setText("Balance: $" + QString::number(balance));
    }

    void resetGame() {
        hitButton->setEnabled(false);
        standButton->setEnabled(false);
        dealButton->setEnabled(true);
        statusLabel->setText("Ready to play? Press Deal.");
    }

    void startRound() {
        currentBet = 100;
        if (balance < currentBet) {
            statusLabel->setText("Not enough money! Game Over.");
            return;
        }

        balance -= currentBet;
        playerHand.clear();
        dealerHand.clear();

        // Deal initial cards
        playerHand.addCard(deck.deal());
        dealerHand.addCard(deck.deal());
        playerHand.addCard(deck.deal());
        dealerHand.addCard(deck.deal());

        gameInProgress = true;
        dealButton->setEnabled(false);
        hitButton->setEnabled(true);
        standButton->setEnabled(true);
        statusLabel->setText("Your Turn...");

        updateUI(false); // Hide dealer's second card

        // Check Natural Blackjack
        if (playerHand.getTotal() == 21) {
            endRound("Blackjack! You Win!", 2.5); // 3:2 payout + original bet return logic
        }
    }

    void onHit() {
        playerHand.addCard(deck.deal());
        updateUI(false);

        if (playerHand.getTotal() > 21) {
            endRound("BUST! Dealer Wins.", 0);
        }
    }

    void onStand() {
        // Dealer Turn
        updateUI(true); // Reveal hole card

        // Simple Loop for Dealer Logic
        while (dealerHand.getTotal() < 17) {
            dealerHand.addCard(deck.deal());
        }
        updateUI(true); // Show final dealer hand

        int pTotal = playerHand.getTotal();
        int dTotal = dealerHand.getTotal();

        if (dTotal > 21) {
            endRound("Dealer Busts! You Win!", 2.0);
        } else if (pTotal > dTotal) {
            endRound("You Win!", 2.0);
        } else if (pTotal < dTotal) {
            endRound("Dealer Wins.", 0);
        } else {
            endRound("Push (Tie).", 1.0);
        }
    }

    void endRound(QString message, double multiplier) {
        gameInProgress = false;
        hitButton->setEnabled(false);
        standButton->setEnabled(false);
        dealButton->setEnabled(true);

        int payout = static_cast<int>(currentBet * multiplier);
        balance += payout;

        statusLabel->setText(message);
        updateUI(true); // Ensure dealer is shown
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BlackjackWindow window;
    window.show();

    return app.exec();
}
