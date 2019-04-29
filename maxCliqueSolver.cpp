#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//Quick way too print vectors
void printVector(vector<int>& passed){
  for (int a=0;a<passed.size();a++){
    cout<<passed[a]<<", ";
  }
}

//Greedy part of the search, find the node with the most connections. From current clique
vector<int> nextBest(vector<int>
  currentClique, vector<vector<int>>& nodeBin, vector<int>& increasing){
  int max = 0;
  int nextBest;
  //find the max value from the increasing list
  for (int j=0;j<increasing.size(); j++){
    if (nodeBin[increasing[j]][0] > max){
      max = nodeBin[increasing[j]][0];
      nextBest = j;
    }
  }
  currentClique.push_back(increasing[nextBest]);
  return currentClique;
}

// Find all valid connections for the current clique
vector<int> increasingList(int n, vector<int>& currentClique, vector<vector<int>>& nodeBin, vector<int>& increasing){
  //if a node = 1
  for (int j=1;j<n+1; j++){
    int count = 0;
    for (int x=0;x<currentClique.size();x++){
      if(nodeBin[currentClique[x]][j] == 1){
        count++;
      }
    }
    if(count == currentClique.size()){
      increasing.push_back(j);
    }
  }
  return increasing;
}

//Turn each node into a binary representation of every connection it makes
vector<int> binaryRepresentation(vector<int>& key, int label, vector<int>& node, vector<int>& val){
  //for the size of nodes to be compared to
  for (int j=0; j<key.size();j++){
    //if node1[i] is matched from any of the node2[j]
    if (key[j] == label){
      node[val[j]] = 1;
    } else if (val[j] == label){
      node[key[j]] = 1;
    }
  }
  return node;
}

//Read the file in between 2 vectors start/ finish
void readInFile(string line, int& n, int& e, vector<int>& node1, vector<int>& node2){
  int temp;
  char delim = ' ';
  //get number of nodes and edges
  if(line[0] == 'p'){
    stringstream ss(line);
    string item;
    int count = 0;
    while(getline(ss, item, delim)){
      if (item[0]>= '0' && item[0] <= '9'){
        if(count == 0){
          istringstream(item) >> n;
          count++;
        }else if (count == 1){
          istringstream(item) >> e;
          count++;
        }
      }
    }

  //get 2 lists representing every connection
  } else if (line[0] == 'e'){
    stringstream ss(line);
    string item;
    int count = 0;
    while(getline(ss, item, delim)){
      if (item[0]>= '0' && item[0] <= '9'){
        if(count == 0){
          istringstream(item) >> temp;
          node1.push_back(temp);
          count++;
        }else if (count == 1){
          istringstream(item) >> temp;
          node2.push_back(temp);
          count++;
        }
      }
    }
  }
}

//Iterate over searches until none left
vector<int> greedySearch(int n, vector<int>& currentClique, vector<vector<int>>& nodeBin, vector<int>& increasing, vector<int>& maxClique){
  while (1){
    //Find all common edges for the currentClique nodes
    increasing = increasingList(n, currentClique, nodeBin, increasing);
    if (increasing.size() == 0){
      break;
    }
    //GREEDY: add node with most connections from increasing list to currentClique
    currentClique = nextBest(currentClique, nodeBin, increasing);
    //clear the increasing list
    increasing.clear();
  }
  return currentClique;
}


//writes total connections into node[0]]
int nodeBinMax(vector<vector<int>>& nodeBin, int n){
  int maximum = 0;
  int placeholder;
  for (int i=0;i<n+1; i++){
    int counter = 0;
    for (int j=0;j<n+1; j++){
      if(nodeBin[i][j] == 1){
        counter++;
      }
      nodeBin[i][0] = counter;
    }
    if (maximum < counter) {
      maximum = counter;
      placeholder = i;
    }
  }
  return placeholder;
}

//resets data for next search
void resetData(vector<int>& currentClique, vector<vector<int>>& nodeBin, vector<int>& increasing, vector<int>& maxClique, vector<vector<int>>& nodeBinOG){
  maxClique.clear();
  currentClique.clear();
  increasing.clear();
  nodeBin = nodeBinOG;
}

//Method to choose a data file to text
string chooseFile(vector<string>& filename){
  cout<<"Please select a data file:"<<endl;
  for(int i=0;i<filename.size();i++){
    cout<<i+1<<". "<<filename[i]<<endl;
  }
  int res;
  cin>>res;
  cout<< filename[res-1]<<".clq selected"<<endl;
  return filename[res-1];
}

// start whole search (iterating through first nodes)
double startSearch(vector<int>& currentClique, vector<vector<int>>& nodeBin, vector<int>& increasing, vector<int>& maxClique, int startNode, vector<int>& largestMax, int n){
  double duration;
  double start;
  start = std::clock();
  //cout<<endl<<"Starting node: "<<startNode<<endl;
  currentClique.push_back(startNode);
  //Search algorithm
  maxClique = greedySearch(n, currentClique, nodeBin, increasing, maxClique);
  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  //print results
  //cout<<"-----------------------------------";
  //cout<<endl<<maxClique.size()<<" - The clique is of size"<<endl<<maxClique[0]<<" - Starting on"<<endl<<duration<<" - time taken"<<endl<<"Containing : ";
  //printVector(maxClique);
  //cout<<endl<<"-----------------------------------"<<endl;

  if (maxClique.size() > largestMax.size()){
    largestMax = maxClique;
  }
return duration;
}


int main(){
  //Declare variables
  srand((unsigned)time(0));
  string line, item;
  int n, e, variance, largestSpace, sumOf, tally, startNode;
  double duration;
  vector<int> node, node1, node2, increasing, currentClique, maxClique, largestMax;
  vector<vector<int>> nodeBin, nodeBinOG;
  vector<string> filename;
  std::clock_t start;


  filename.push_back("brock800_1");
  filename.push_back("brock800_2");
  filename.push_back("brock800_3");
  filename.push_back("brock800_4");
  filename.push_back("C2000.9");
  filename.push_back("C4000.5");
  filename.push_back("MANN_a45");
  filename.push_back("p_hat1500-1");
  //get user input for file selection
  string inName = chooseFile(filename);
  ifstream myfile( "InputData/" + inName +".clq" );


  //read file into node1, node2
  if (myfile){
    while (getline( myfile, line )){
      readInFile(line, n, e, node1, node2);
    }
    myfile.close();
  }

  //make binary representation of each nodes connections
  for (int i=0; i<n+1;i++){
    //for all possible nodes make a 0 representation
    node.clear();
    for(int z=0; z<n+1;z++){
      node.push_back(0);
    }
    vector<int> tempo = binaryRepresentation(node1, i, node, node2); //possibly get rid of tempo and use node
    nodeBin.push_back(tempo);
  }

  //add how many connections in each node as the first value index[0]
  int maximum = nodeBinMax(nodeBin, n);
  //copy node bin before editing
  nodeBinOG = nodeBin;
  //find average
  //find max for some random searches
  largestSpace = 0;
  sumOf = 0;
  for (int j=1; j<n+1; j++){
    sumOf =  sumOf + nodeBin[j][0];
    if (nodeBin[j][0] > largestSpace){
      largestSpace = nodeBin[j][0];
    }
  }
  sumOf = sumOf/n+1;
  //get a 10% variance
  variance = sumOf / 10;
  //choose random nodes and check if they fit with the variance
  tally = 0;
  while(1){
    startNode = rand() % n + 1;
    //every 10 iterations choose from the largest search space - variance
    if (tally % 10 == 0){
      if (nodeBin[startNode][0] > largestSpace - variance){
        resetData(currentClique, nodeBin, increasing, maxClique, nodeBinOG);
        startSearch(currentClique, nodeBin,  increasing,  maxClique, startNode, largestMax,n);
        tally++;
      }
    }
    //otherwise search within 20% of the average search space
    else if (nodeBin[startNode][0] < sumOf + variance && nodeBin[startNode][0] > sumOf - variance){
      resetData(currentClique, nodeBin, increasing, maxClique, nodeBinOG);
      startSearch(currentClique, nodeBin,  increasing,  maxClique, startNode, largestMax,n);
      tally++;
    }
    if (tally == 100){
      break;
    }
  }
  // print out best result
  cout<<"-----------------------------------";
  cout<<endl<<"The largest maximum clique is of size: "<<largestMax.size()<<endl<<"Containing : ";
  printVector(largestMax);
  cout<<endl<<"-----------------------------------"<<endl<<endl;
  return 0;
}
