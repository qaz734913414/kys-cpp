#include "TeamMenu.h"
#include "Save.h"
#include "Button.h"
#include "GameUtil.h"

TeamMenu::TeamMenu()
{
    for (int i = 0; i < TEAMMATE_COUNT; i++)
    {
        auto h = new Head();
        h->setHaveBox(false);
        //h->setOnlyHead(true);
        heads_.push_back(h);
        addChild(h, i % 2 * 250, i / 2 * 100);
        //selected_.push_back(0);
    }
    button_all_ = new Button();
    button_all_->setText("ȫ�x");
    button_ok_ = new Button();
    button_ok_->setText("�_��");
    addChild(button_all_, 50, 300);
    addChild(button_ok_, 150, 300);
    setPosition(200, 150);
    setTextPosition(20, -30);
    mode_ = 1;
}

TeamMenu::~TeamMenu()
{
}

void TeamMenu::onEntrance()
{
    for (int i = 0; i < TEAMMATE_COUNT; i++)
    {
        auto r = Save::getInstance()->getTeamMate(i);
        if (r)
        {
            heads_[i]->setRole(r);
            if (mode_ == 0 && item_)
            {
                if (!GameUtil::canUseItem(r, item_))
                {
                    heads_[i]->setText("���m��");

                }
                if (r->PracticeItem == item_->ID || r->Equip0 == item_->ID || r->Equip1 == item_->ID)
                {
                    heads_[i]->setText("ʹ����");
                }
            }
        }
    }
    if (mode_ == 0)
    {
        button_all_->setVisible(false);
        button_ok_->setVisible(false);
    }
}

Role* TeamMenu::getRole()
{
    return role_;
}

std::vector<Role*> TeamMenu::getRoles()
{
    std::vector<Role*> roles;
    for (auto h : heads_)
    {
        if (h->getResult() == 0)
        {
            roles.push_back(h->getRole());
        }
    }
    return roles;
}

void TeamMenu::draw()
{
    Engine::getInstance()->fillColor({ 0, 0, 0, 192 }, 0, 0, -1, -1);
    TextBox::draw();
}

void TeamMenu::pressedOK()
{
    if (mode_ == 0)
    {
        role_ = nullptr;
        for (auto h : heads_)
        {
            if (h->getState() == Press)
            {
                role_ = h->getRole();
            }
        }
        if (role_)
        {
            result_ = 0;
            setExit(true);
        }
    }
    if (mode_ == 1)
    {
        for (auto h : heads_)
        {
            if (h->getState() == Press)
            {
                if (h->getResult() == -1)
                {
                    h->setResult(0);
                }
                else
                {
                    h->setResult(-1);
                }
            }
        }
        if (button_all_->getState() == Press)
        {
            for (auto h : heads_)
            {
                h->setResult(0);
            }
        }
        if (button_ok_->getState() == Press)
        {
            for (auto h : heads_)
            {
                if (h->getResult() == 0)
                {
                    setExit(true);
                }
            }
        }
    }
}

void TeamMenu::pressedCancel()
{
    if (mode_ == 0)
    {
        role_ = nullptr;
        result_ = -1;
        setExit(true);
    }
}

void TeamMenu::dealEvent(BP_Event& e)
{
    if (mode_ == 0)
    {
        if (item_)
        {
            for (auto h : heads_)
            {
                if (h->getState() != Normal && !GameUtil::canUseItem(h->getRole(), item_))
                {
                    h->setState(Normal);
                }
            }
        }
    }
    if (mode_ == 1)
    {
        for (auto h : heads_)
        {
            if (h->getResult() == 0)
            {
                h->setText("���x��");
            }
            else
            {
                h->setText("");
            }
        }
    }
}