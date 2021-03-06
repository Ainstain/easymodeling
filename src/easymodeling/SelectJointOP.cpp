/*
* Copyright (c) 2013 Guang Zhu http://runnersoft.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "SelectJointOP.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "JointPropertySetting.h"
#include "WorldPropertySetting.h"

using namespace emodeling;

SelectJointOP::SelectJointOP(d2d::EditPanel* editPanel, 
							 d2d::MultiSpritesImpl* spritesImpl, 
							 d2d::PropertySettingPanel* propertyPanel, 
							 d2d::AbstractEditCMPT* callback /*= NULL*/)
	: SelectBodyOP(editPanel, spritesImpl, propertyPanel, callback)
	, m_mouseOn(NULL)
	, m_selected(NULL)
{
}

bool SelectJointOP::onKeyDown(int keyCode)
{
	if (SelectBodyOP::onKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_selected)
	{
		if (m_mouseOn == m_selected) m_mouseOn = NULL;
		static_cast<StagePanel*>(m_editPanel)->removeJoint(m_selected);
		m_selected = NULL;
		m_editPanel->Refresh();
	}

	return false;
}

bool SelectJointOP::onMouseLeftDown(int x, int y)
{
	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	libmodeling::Joint* selected = static_cast<StagePanel*>(m_editPanel)->queryJointByPos(pos);
	if (selected && !m_selected || !selected && m_selected)
		m_editPanel->Refresh();
	if (selected)
	{
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (jointSelection.isExist(selected))
				jointSelection.erase(selected);
			else
				jointSelection.insert(selected);
		}
		else
		{
			if (!jointSelection.isExist(selected))
			{
				jointSelection.clear();
				jointSelection.insert(selected);
			}
		}

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		m_selection->clear();
	}

	m_selected = selected;

	if (m_selected)
	{
		m_propertyPanel->setPropertySetting(new JointPropertySetting(m_editPanel, m_selected));
//		m_selection->clear();
	}
	else
	{
		m_propertyPanel->setPropertySetting(new WorldPropertySetting(m_editPanel));
		SelectBodyOP::onMouseLeftDown(x, y);
	}

	m_firstPos = pos;

	return false;
}

bool SelectJointOP::onMouseLeftUp(int x, int y)
{
	//d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	//selectedJoints.clear();
	//static_cast<StagePanel*>(m_editPanel)->queryJointsByRect(d2d::Rect(pos, m_firstPos), selectedJoints);
	//if (selectedJoints.size() == 1)
	//	m_propertyPanel->setPropertySetting(new JointPropertySetting(m_editPanel, selectedJoints[0]));

	if (SelectBodyOP::onMouseLeftUp(x, y)) return true;

	if (m_selected)
		m_propertyPanel->setPropertySetting(new JointPropertySetting(m_editPanel, m_selected));
	else
		m_propertyPanel->setPropertySetting(new WorldPropertySetting(m_editPanel));

	return false;
}

bool SelectJointOP::onMouseMove(int x, int y)
{
	if (SelectBodyOP::onMouseMove(x, y)) 
		return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	libmodeling::Joint* joint = static_cast<StagePanel*>(m_editPanel)->queryJointByPos(pos);
	if (joint && !m_mouseOn || !joint && m_mouseOn)
		m_editPanel->Refresh();

	m_mouseOn = joint;

	return false;
}

bool SelectJointOP::onMouseDrag(int x, int y)
{
	if (SelectBodyOP::onMouseDrag(x, y)) 
		return true;

	if (m_selected)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		switch (m_selected->type)
		{
		case libmodeling::Joint::e_revoluteJoint:
			{
				libmodeling::RevoluteJoint* joint = static_cast<libmodeling::RevoluteJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_prismaticJoint:
			{
				libmodeling::PrismaticJoint* joint = static_cast<libmodeling::PrismaticJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_distanceJoint:
			{
				libmodeling::DistanceJoint* joint = static_cast<libmodeling::DistanceJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_pulleyJoint:
			{
				libmodeling::PulleyJoint* joint = static_cast<libmodeling::PulleyJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				const float disGA = d2d::Math::getDistance(pos, joint->groundAnchorA),
					disGB = d2d::Math::getDistance(pos, joint->groundAnchorB);

				float dis = std::min(std::min(disA, disB), std::min(disGA, disGB));
				if (dis == disA)
					joint->setLocalAnchorA(pos);
				else if (dis == disB)
					joint->setLocalAnchorB(pos);
				else if (dis == disGA)
					joint->groundAnchorA = pos;
				else
					joint->groundAnchorB = pos;					
			}
			break;
		case libmodeling::Joint::e_gearJoint:
			{
			}
			break;
		case libmodeling::Joint::e_wheelJoint:
			{
				libmodeling::WheelJoint* joint = static_cast<libmodeling::WheelJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_weldJoint:
			{
				libmodeling::WeldJoint* joint = static_cast<libmodeling::WeldJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_frictionJoint:
			{
				libmodeling::FrictionJoint* joint = static_cast<libmodeling::FrictionJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_ropeJoint:
			{
				libmodeling::RopeJoint* joint = static_cast<libmodeling::RopeJoint*>(m_selected);
				const float disA = d2d::Math::getDistance(pos, joint->getWorldAnchorA()),
					disB = d2d::Math::getDistance(pos, joint->getWorldAnchorB());
				if (disA < disB)
					joint->setLocalAnchorA(pos);
				else
					joint->setLocalAnchorB(pos);
			}
			break;
		case libmodeling::Joint::e_motorJoint:
			{
			}
			break;
		}

		m_editPanel->Refresh();
	}

	return false;
}

bool SelectJointOP::onDraw() const
{
	if (SelectBodyOP::onDraw()) 
		return true;

	m_selection->traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		m_mouseOn->draw(libmodeling::Joint::e_mouseOn);
	if (m_selected) 
		m_selected->draw(libmodeling::Joint::e_selected);

	return false;
}

bool SelectJointOP::clear()
{
	if (SelectBodyOP::onDraw()) 
		return true;

	m_selected = m_mouseOn = NULL;

	return false;
}

d2d::IPropertySetting* SelectJointOP::createPropertySetting(d2d::ISprite* sprite) const
{
	return NULL;
}

void SelectJointOP::DrawSelectedVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext) 
{
	std::vector<d2d::Vector> bound;
	d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
	libmodeling::Body* body = static_cast<libmodeling::Body*>(sprite->getUserData());
	DrawUtils::drawBody(body, DrawUtils::e_selected);
	bFetchNext = true;
}
