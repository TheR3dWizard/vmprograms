import sys
import random as r

class DeckDealer:
    def __init__(self,numplayers,round = 1):
        self.deck = [i for i in range(1,101)]
        self.numplayers = numplayers
        self.round = round

    def deal(self):
        self.deck = [i for i in range(1,101)]
        r.shuffle(self.deck)
        hands = []
        for i in range(self.numplayers):
            hands.append(self.deck[0:self.round])
            self.deck = self.deck[self.round:]
        return hands
    
    def gethands(self):
        with open("deck.txt","w") as f:
            for hand in self.deal():
                string = "".join([str(i) + " " for i in hand])
                print(string)
    


    def __str__(self):
        return str(self.deck)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 deckdealer.py <numplayers> <round>")
        sys.exit()

    numplayers = int(sys.argv[1])
    round = int(sys.argv[2])
    dealer = DeckDealer(numplayers,round)
    dealer.gethands()