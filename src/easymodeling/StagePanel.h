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

#ifndef EMODELING_STAGE_PANEL_H
#define EMODELING_STAGE_PANEL_H

#include <drag2d.h>

namespace libmodeling
{
	class Body;
	class Joint;
}

namespace emodeling
{
	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent);
		virtual ~StagePanel();

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::SpritesPanelImpl interface
		//
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual d2d::ISprite* querySpriteByPos(const d2d::Vector& pos) const;
		virtual void querySpritesByRect(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result) const;		

		libmodeling::Joint* queryJointByPos(const d2d::Vector& pos) const;
		void queryJointsByRect(const d2d::Rect& rect, std::vector<libmodeling::Joint*>& result) const;

		void insertJoint(libmodeling::Joint* joint) {
			m_joints.push_back(joint);
		}
		void removeJoint(libmodeling::Joint* joint);

		void traverseBodies(d2d::IVisitor& visitor) const;
		void traverseJoints(d2d::IVisitor& visitor) const;

	private:
		static void loadBody(const wxString& filepath, libmodeling::Body& body);
		static void loadBody(d2d::ISprite* sprite, libmodeling::Body& body);

	private:
		class PointQueryVisitor : public d2d::IVisitor
		{
		public:
			PointQueryVisitor(const d2d::Vector& pos, d2d::ISprite** pResult);
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		private:
			const d2d::Vector& m_pos;
			d2d::ISprite** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public d2d::IVisitor
		{
		public:
			RectQueryVisitor(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result);
			virtual void visit(d2d::ICloneable* object, bool& bFetchNext);

		private:
			const d2d::Rect& m_rect;
			std::vector<d2d::ISprite*>& m_result;

		}; // RectQueryVisitor

	private:
		std::vector<libmodeling::Body*> m_bodies;
		std::vector<libmodeling::Joint*> m_joints;

	}; // StagePanel
}

#endif // EMODELING_STAGE_PANEL_H
