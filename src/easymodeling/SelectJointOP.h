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

#ifndef EMODELING_SELECT_JOINT_OP_H
#define EMODELING_SELECT_JOINT_OP_H

#include "SelectBodyOP.h"

namespace libmodeling { class Joint; }

namespace emodeling
{
	class StagePanel;

	class SelectJointOP : public SelectBodyOP
	{
	public:
		SelectJointOP(d2d::EditPanel* editPanel, 
			d2d::MultiSpritesImpl* spritesImpl, 
			d2d::PropertySettingPanel* propertyPanel, 
			d2d::AbstractEditCMPT* callback = NULL);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		virtual d2d::IPropertySetting* createPropertySetting(d2d::ISprite* sprite) const;

	private:
		class DrawSelectedVisitor : public d2d::IVisitor
		{
		public:
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		}; // DrawSelectedVisitor

	private:
		libmodeling::Joint* m_selected;
		libmodeling::Joint* m_mouseOn;

		d2d::Vector m_firstPos;

	public:
		d2d::SelectionSet<libmodeling::Joint> jointSelection;

//		std::vector<Joint*> selectedJoints;

	}; // SelectJointOP
}

#endif // EMODELING_SELECT_JOINT_OP_H
