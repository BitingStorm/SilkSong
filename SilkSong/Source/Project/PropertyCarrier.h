#pragma once


class PropertyComponent;

class IPropertyCarrier
{
public:
	virtual PropertyComponent* GetProperty() = 0;
};