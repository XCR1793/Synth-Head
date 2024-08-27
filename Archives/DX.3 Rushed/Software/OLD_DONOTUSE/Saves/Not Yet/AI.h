#pragma once

/**
 * @brief Shit to do
 * Activation Functions (Switch Block)
 * Array Initialization
 * Back Propagation
 * Neuron Computation
 * Object based AI creation (for multiple AIs)
 * Spitout Internal Data
 * Error Calculation
 * Active Debug Output
 */

class AI{
    private:
    int* CreateArray1D(int size){
        int* NewArray = new int[size];
        for(int i = 0; i < size; i++){
            NewArray[i] = 0;
        }
        return NewArray;
    }
    int** CreateArray2D(int size1, int size2){
        int** NewArray = new int*[size1];
        for(int i = 0; i < size1; i++){
            for(int j = 0; j < size2; j++){
                NewArray[i][j] = 0;
            }
        }
    }
    int*** CreateArray3D(int size1, int size2, int size3){
        int*** NewArray = new int**[size1];
        for(int i = 0; i < size1; i++){
            for(int j = 0; j < size2; j++){
                for(int k = 0; k < size3; k++){
                    NewArray[i][j][k] = 0;
                }
            }
        }

    }

    int* NodesInput;
    int** NodesHidden;
    int** NodesBypass;
    int*** Weights;

    public:

    // All variables that require a value
    int InputNodes = 0;
    int OutputNodes = 0;
    int HiddenLayers = 0;
    int HiddenNodes = 0;

    // Activation Function
    double ActivationFunction(int FunctionSelection, double InputSum, int gradient){
        switch(FunctionSelection){
        case 1: // Relu
            if(InputSum){return 0;}else{return InputSum;}
            break;
        case 2: // Leaky Relu
            if(InputSum){return (InputSum * gradient);}else{return InputSum;}
            break;
        default:
            break;
        }
    }

    void Create(){
        NodesInput = CreateArray1D(InputNodes);
        NodesHidden = CreateArray2D(HiddenLayers, HiddenNodes);
        NodesBypass = CreateArray2D(HiddenLayers, HiddenNodes);
        Weights = CreateArray3D(HiddenLayers + 2, HiddenNodes, HiddenNodes);

        for(size_t i = 0; i < HiddenLayers; i++){
            for(size_t j = 0; j < HiddenNodes; j++){
                NodesBypass[i][j] = 1;
            }
        }
    }
    
    void LayerRestrict(int LayerSelected, int MaxNodes){
        NodesBypass[LayerSelected][MaxNodes] = 0;
    }

    // Neurons

    // Weights
    // Biases
};

/** Example of usage

    AI Caw; Creates CAW AI as an object
    Caw.InputNodes(input number amount); Sets Input layer node quantity
    Caw.OutputNodes(output number amount); Sets Output layer node quantity
    Caw.HiddenLayers(Number of hidden layers, Max size); Creates 2D array of hidden layer quantity and maximum nodes in hidden layer

    Caw.Create(); Converts all variables set above into arrays that dont change in size
Caw.LayerRestrict(Layer number, Nodes); Restricts max nodes per layer

 */