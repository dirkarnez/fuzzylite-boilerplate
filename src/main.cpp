#include "fl/Headers.h"

int main(int argc, char** argv){
    FL_IUNUSED(argc);
    FL_IUNUSED(argv);
    using namespace fl;

    // 300 - 850, 660
    Engine* engine = new Engine;
    engine->setName("ObstacleAvoidance");
    engine->setDescription("");

    InputVariable* credit = new InputVariable;
    credit->setName("credit");
    credit->setDescription("");
    credit->setEnabled(true);
    credit->setRange(300.000, 850.000);
    credit->setLockValueInRange(false);
    //credit->addTerm(new Ramp("bad", 550.000, 650.000, 100.000));
    credit->addTerm(new Ramp("bad", 650.000, 550.000, 100.000));
    credit->addTerm(new Triangle("neutral", 550.000, 650.000, 750.000, 100.000));
    credit->addTerm(new Ramp("good", 650.000, 750.000, 100.000));
    engine->addInputVariable(credit);

    OutputVariable* risk = new OutputVariable;
    risk->setName("risk");
    risk->setDescription("");
    risk->setEnabled(true);
    risk->setRange(0.000, 100.000);
    risk->setLockValueInRange(false);
    risk->setAggregation(new Maximum);
    risk->setDefuzzifier(new Centroid);
    risk->setDefaultValue(fl::nan);
    risk->setLockPreviousValue(false);
    //risk->addTerm(new Ramp("low", 25.000, 50.000, 100.000));
    risk->addTerm(new Ramp("low", 50.000, 25.000, 100.000));
    risk->addTerm(new Triangle("medium", 25.000, 50.000, 75.000, 100.000));
    risk->addTerm(new Ramp("high", 50.000, 75.000, 100.000));
    engine->addOutputVariable(risk);

    RuleBlock* mamdani = new RuleBlock;
    mamdani->setName("mamdani");
    mamdani->setDescription("");
    mamdani->setEnabled(true);
    mamdani->setConjunction(fl::null);
    mamdani->setDisjunction(fl::null);
    mamdani->setImplication(new AlgebraicProduct);
    mamdani->setActivation(new General);
    mamdani->addRule(Rule::parse("if credit is bad then risk is high", engine));
    mamdani->addRule(Rule::parse("if credit is neutral then risk is medium", engine));
    mamdani->addRule(Rule::parse("if credit is good then risk is low", engine));

    engine->addRuleBlock(mamdani);

    std::string status;
    if (not engine->isReady(&status)) {
        std::cout << "[engine error] engine is not ready:\n";
        std::cout << status << std::endl;
        //std::cout << FL_AT << std::endl;
    }

    // for (int i = 0; i <= 50; ++i){
    //     scalar location = credit->getMinimum() + i * (credit->range() / 50);
    //     credit->setValue(location);
    //     engine->process();
    //     FL_LOG("obstacle.input = " << Op::str(location) << 
    //         " => " << "steer.output = " << Op::str(risk->getValue()));
    // }

    credit->setValue(660.000);
    engine->process();
    FL_LOG("credit.input = " << Op::str(660.000) << 
        " => " << "risk.output = " << Op::str(risk->getValue()));

    std::cin.get();
    return 0;
}