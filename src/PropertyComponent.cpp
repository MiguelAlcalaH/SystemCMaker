#include <PropertyComponent.h>

PropertyComponent::PropertyComponent(const QString &n) : name(n)
{
    if(!n.isEmpty())
    {
        addPort("a_in",PortItem::IN);
        addPort("b_in",PortItem::IN);
        addPort("c_out",PortItem::OUT);
    }

}

PropertyComponent::PropertyComponent(const QString &n,int tp) : name(n)
{
    if(!n.isEmpty())
    {
        addPort("a_in",PortItem::IN);
        addPort("b_in",PortItem::IN);
        addPort("c_out",PortItem::OUT);
        this->type = tp;
    }

}


void PropertyComponent::addPort(const QString &nPort,PortItem::Sig s)
{
    if(s == PortItem::IN)
    {
        inPorts << nPort;
        return;
    }

    outPorts << nPort;
}

void PropertyComponent::setPos(const QPointF &p)
{
    pos = p;
}

int PropertyComponent::numberInPorts() const
{
    return inPorts.size();
}

int PropertyComponent::numberOutPorts() const
{
    return outPorts.size();
}

void PropertyComponent::clear()
{
    inPorts.clear();
    outPorts.clear();
}
PropertyComponent PropertyComponent::getAndGate()
{
    PropertyComponent gate("AND_gate");

    return gate;
}

PropertyComponent PropertyComponent::getOrGate()
{
    PropertyComponent gate("OR_gate");

    return gate;
}

PropertyComponent PropertyComponent::getXorGate()
{
    PropertyComponent gate("XOR_gate");
    return gate;
}

PropertyComponent PropertyComponent::getNotGate()
{
    PropertyComponent gate("NOT_gate");
    gate.clear();
    gate.addPort("a_in",PortItem::IN);
    gate.addPort("b_out",PortItem::OUT);
    return gate;
}

void PropertyComponent::operator=(const PropertyComponent &other)
{
    name = other.name;
    pos = other.pos;
    inPorts = other.inPorts;
    outPorts = other.outPorts;
}
