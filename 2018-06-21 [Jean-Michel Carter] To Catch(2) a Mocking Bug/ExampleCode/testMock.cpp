#include "catch.hpp"
#include "fakeit.hpp"

class IRelay{
public:
    virtual int activate() = 0;
    virtual int deactivate() = 0;
    virtual ~IRelay(){};
};

class Compressor{
    IRelay* relay;
public:
    void turnOn() {
        if (relay)
            relay->activate();
    }

    void turnOff() {
        if (relay)
            relay->deactivate();
    }

    void setRelay(IRelay* relay_){
        relay = relay_;
    }
};

TEST_CASE("Testing Mocks"){

    Compressor comp;
    fakeit::Mock<IRelay> mockComp;
    fakeit::When(Method(mockComp,activate)).AlwaysReturn(1);
   fakeit::Fake(Method(mockComp,deactivate));

    comp.setRelay(&mockComp.get());

    SECTION("When comp gets turned On, relay.activate gets called once"){
        comp.turnOn();
        fakeit::Verify(Method(mockComp,activate)).Exactly(1);
        fakeit::Verify(Method(mockComp,deactivate)).Exactly(0);

       SECTION("And When comp gets turned off, relay.deactivate gets called once"){
           comp.turnOff();
           fakeit::VerifyNoOtherInvocations(Method(mockComp,activate));
           fakeit::Verify(Method(mockComp,deactivate)).Exactly(1);
       }
    }


}