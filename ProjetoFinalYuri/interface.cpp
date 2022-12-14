#include "interface.h"
#include "character.h"
#include "skillsattribute.h"
#include "barraprogresso.h"

#include <QtWidgets>
#include <QGridLayout>
#include <QLineEdit>
#include <QtMath>
#include <QApplication>
#include <QStyle>


Interface::Interface(QWidget *parent) : QWidget(parent)
{
    //Titulo da Janela
    this->setWindowTitle("Jogo");
    setFixedSize(600,600); // Tamanho da Janela
    QPixmap bkgnd(":/Background.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setStyleSheet("QLabel { color : white; }");
    this->setPalette(palette);

    //--------INICIALIZAÇÃO DO PERSONAGEM---------
    _player = new Character(1000, 360, 50, 32, "Player", ":/Outfit/Mage_Male_Full.png");
    _player->setWeapon(new Weapon(32));
    _player->setShield(new Shield(0.05));
    qtd_HpPot = 5;
    qtd_UH = 25;
    qtd_manaPot = 100;
    qtd_bigmanaPot = 20;

    _monster.push_back(new Character(100,1, 1, 12, "Rat", ":/Monsters/Rat.png"));
    _monster.push_back(new Character(100,1, 1, 15, "Troll", ":/Monsters/Rat.png"));
    _monster.push_back(new Character(100,1, 1, 19, "Orc", ":/Monsters/Orc.png"));
    _monster.push_back(new Character(1000,1, 1, 25, "Dragon", ":/Monsters/Dragon.png"));
    _monster.push_back(new Character(3600,1, 1, 40, "Hydra", ":/Monsters/Hydra.png"));
    _monster.push_back(new Character(8000,1, 1, 55, "Demon", ":/Monsters/Demon.png"));

    _monster[0]->setShield(new Shield(0.05));
    _monster[0]->setWeapon(new Weapon(10));

    _monster[1]->setShield(new Shield(0.05));
    _monster[1]->setWeapon(new Weapon(15));

    _monster[2]->setShield(new Shield(0.05));
    _monster[2]->setWeapon(new Weapon(20));

    _monster[3]->setShield(new Shield(0.05));
    _monster[3]->setWeapon(new Weapon(30));

    _monster[4]->setShield(new Shield(0.05));
    _monster[4]->setWeapon(new Weapon(40));

    _monster[5]->setShield(new Shield(0.05));
    _monster[5]->setWeapon(new Weapon(50));

    _enemy = _monster[5];
    //_enemy = new Character(100,1, 12, "Rat", ":/Monsters/Rat.png"); //&_monster[0];
    //_enemy->setShield(new Shield(0.05));
    //_enemy->setWeapon(new Weapon(10));

    //-------FIM DA CRIAÇÃO DO PERSONAGEM----------


    //-----------BUTTON TAB----------------------------------------------------------------------------------
    // Display é o campo onde fica o texto
    consoleTxt = new QTextEdit("Welcome to the game:");
    consoleTxt->setAlignment(Qt::AlignHCenter);
    consoleTxt->setReadOnly(true);
    endingTxt = new QLabel("");
    endingTxt->setEnabled(false);
    endingTxt->setAlignment(Qt::AlignHCenter);

    //consoleTxt->append("test");
    //display->setFixedHeight(600);

    button_atq = new QPushButton("Attack");
    QObject::connect(button_atq, SIGNAL(clicked()),
                     this, SLOT(duel()));

    button_def = new QPushButton("Defend");
    QObject::connect(button_def, SIGNAL(clicked()),
                     this, SLOT(duel()));
    //-------------------------------------------------------------------------------------------------------

    //-----------ATRIBUTOS TAB-------------------------------------------------------------------------------
    // Atributos para skill table
    skillsAttribute * level = new skillsAttribute("Level:", _player->getLevel(), true, 50);
    skillsAttribute * hitpoints = new skillsAttribute("Hitpoints:", _player->getLifeLevel(), true, 1000);
    skillsAttribute * mana = new skillsAttribute("Mana:", _player->getMana(), true, 360);

    QObject::connect(_player, SIGNAL(lifeChanged(int)), hitpoints, SLOT(updateValue(int)));
    QObject::connect(_player, SIGNAL(levelChanged(int)), level, SLOT(updateValue(int)));
    QObject::connect(_player, SIGNAL(manaChanged(int)), mana, SLOT(updateValue(int)));

    button_exura = new QPushButton("Magia: exura");
    QObject::connect(button_exura, SIGNAL(clicked()),
                     this, SLOT(exura()));


    //--------------------------------------------------------------------------------------------------------

    //-----------GAMEWINDOW TAB-------------------------------------------------------------------------------
    skillsAttribute * enemy_hitpoints = new skillsAttribute("Enemy Life:", _enemy->getLifeLevel(), true, 8000);
    skillsAttribute * player_hitpoints = new skillsAttribute("Player Life:", _player->getLifeLevel(), true, 1000);
    QPixmap player_outfit(":/Outfit/Slayer.png");
    QPixmap enemy_outfit(_enemy->sprite());
    QLabel *enemy_sprite = new QLabel("");
    enemy_sprite->setPixmap(enemy_outfit);
    enemy_sprite->setBaseSize(100,100);
    QLabel *player_sprite = new QLabel("");
    player_sprite->setPixmap(player_outfit);
    player_sprite->setBaseSize(100,100);
    QObject::connect(_enemy, SIGNAL(lifeChanged(int)), enemy_hitpoints, SLOT(updateValue(int)));
    QObject::connect(_player, SIGNAL(lifeChanged(int)), player_hitpoints, SLOT(updateValue(int)));
    //--------------------------------------------------------------------------------------------------------

    //-----------EQUIPMENT TAB-------------------------------------------------------------------------------
    QLabel *equipament_label = new QLabel("<h2>Equipment</h2>");
    QLabel *backpack_label = new QLabel("<h2>Backpack</h2>");
    equipament_label->setFixedHeight(20);
    backpack_label->setFixedHeight(20);

    btn_necklace = new QPushButton("");
  //  btn_necklace->setFixedSize(40,40);
    QPixmap necklace_sprite(":/Itens/aol.png");
    QIcon necklace_icon(necklace_sprite);
    btn_necklace->setIcon(necklace_icon);
    btn_necklace->setIconSize(necklace_sprite.rect().size());

    btn_helmet = new QPushButton("");
   // btn_helmet->setFixedSize(40,40);
    QPixmap helmet_sprite(":/Itens/demon_helmet.png");
    QIcon helmet_icon(helmet_sprite);
    btn_helmet->setIcon(helmet_icon);
    btn_helmet->setIconSize(helmet_sprite.rect().size());

    btn_backpack = new QPushButton("");
  //  btn_backpack->setFixedSize(40,40);
    QPixmap backpack_sprite(":/Itens/bp_poi.png");
    QIcon backpack_icon(backpack_sprite);
    btn_backpack->setIcon(backpack_icon);
    btn_backpack->setIconSize(helmet_sprite.rect().size());

    btn_leftHand = new QPushButton("");
  //  btn_leftHand->setFixedSize(40,40);
    QPixmap leftHand_sprite(":/Itens/Fire_Sword.gif");
    QIcon leftHand_icon(leftHand_sprite);
    btn_leftHand->setIcon(leftHand_icon);
    btn_leftHand->setIconSize(leftHand_sprite.rect().size());

    btn_armor = new QPushButton("");
   // btn_armor->setFixedSize(40,40);
    QPixmap armor_sprite(":/Itens/mpa.png");
    QIcon armor_icon(armor_sprite);
    btn_armor->setIcon(armor_icon);
    btn_armor->setIconSize(armor_sprite.rect().size());

    btn_rightHand = new QPushButton("");
  //  btn_rightHand->setFixedSize(40,40);
    QPixmap rightHand_sprite(":/Itens/mms.png");
    QIcon rightHand_icon(rightHand_sprite);
    btn_rightHand->setIcon(rightHand_icon);
    btn_rightHand->setIconSize(rightHand_sprite.rect().size());

    btn_ring = new QPushButton("");
  //  btn_ring->setFixedSize(40,40);
    QPixmap ring_sprite(":/Itens/gold_ring.png");
    QIcon ring_icon(ring_sprite);
    btn_ring->setIcon(ring_icon);
    btn_ring->setIconSize(ring_sprite.rect().size());

    btn_boots = new QPushButton("");
   // btn_boots->setFixedSize(40,40);
    QPixmap boots_sprite(":/Itens/boh.png");
    QIcon boots_icon(boots_sprite);
    btn_boots->setIcon(boots_icon);
    btn_boots->setIconSize(boots_sprite.rect().size());

    btn_utility = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap utility_sprite(":/Itens/frozen.png");
    QIcon utility_icon(utility_sprite);
    btn_utility->setIcon(utility_icon);
    btn_utility->setIconSize(utility_sprite.rect().size());

    backpackSlot1 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot1_sprite(":/Itens/hp_pot.png");
    QIcon bpslot1_icon(bpslot1_sprite);
    backpackSlot1->setIcon(bpslot1_icon);
    backpackSlot1->setIconSize(bpslot1_sprite.rect().size());
    QObject::connect(backpackSlot1, SIGNAL(clicked()),
                     this, SLOT(healingPOT()));

    backpackSlot2 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot2_sprite(":/Itens/rune_uh.png");
    QIcon bpslot2_icon(bpslot2_sprite);
    backpackSlot2->setIcon(bpslot2_icon);
    backpackSlot2->setIconSize(bpslot2_sprite.rect().size());
    QObject::connect(backpackSlot2, SIGNAL(clicked()),
                     this, SLOT(healingUH()));

    backpackSlot3 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot3_sprite(":/Itens/canivete.png");
    QIcon bpslot3_icon(bpslot3_sprite);
    backpackSlot3->setIcon(bpslot3_icon);
    backpackSlot3->setIconSize(bpslot3_sprite.rect().size());

    backpackSlot4 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot4_sprite(":/Itens/rune_explo.png");
    QIcon bpslot4_icon(bpslot4_sprite);
    backpackSlot4->setIcon(bpslot4_icon);
    backpackSlot4->setIconSize(bpslot4_sprite.rect().size());

    backpackSlot5 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot5_sprite(":/Itens/big_manapot.png");
    QIcon bpslot5_icon(bpslot5_sprite);
    backpackSlot5->setIcon(bpslot5_icon);
    backpackSlot5->setIconSize(bpslot5_sprite.rect().size());
    QObject::connect(backpackSlot5, SIGNAL(clicked()),
                     this, SLOT(bigmanaPOT()));

    backpackSlot6 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot6_sprite(":/Itens/coin.png");
    QIcon bpslot6_icon(bpslot6_sprite);
    backpackSlot6->setIcon(bpslot6_icon);
    backpackSlot6->setIconSize(bpslot6_sprite.rect().size());

    backpackSlot7 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot7_sprite(":/Itens/mana_pot.png");
    QIcon bpslot7_icon(bpslot7_sprite);
    backpackSlot7->setIcon(bpslot7_icon);
    backpackSlot7->setIconSize(bpslot7_sprite.rect().size());
    QObject::connect(backpackSlot7, SIGNAL(clicked()),
                     this, SLOT(manaPOT()));

    backpackSlot8 = new QPushButton("");
   // btn_utility->setFixedSize(40,40);
    QPixmap bpslot8_sprite(":/Itens/f_hat.png");
    QIcon bpslot8_icon(bpslot8_sprite);
    backpackSlot8->setIcon(bpslot8_icon);
    backpackSlot8->setIconSize(bpslot8_sprite.rect().size());

    //--------------------------------------------------------------------------------------------------------

    // Layout para a janela principal
    mainUI = new QHBoxLayout;

    // PARA TABELA DE SKILLS
    skillsTab = new QVBoxLayout;
    skillsTab->addWidget(level);
    skillsTab->addWidget(hitpoints);
    skillsTab->addWidget(mana);
    skillsTab->addWidget(button_exura);
    skillsTab->addStretch();

    buttonBox = new QHBoxLayout;
    buttonBox->addWidget(button_atq);
    buttonBox->addWidget(button_def);

    buttonsTab = new QVBoxLayout;
    enemyHUD = new QHBoxLayout;
    enemyHUD->addWidget(enemy_hitpoints);
    enemyHUD->addWidget(enemy_sprite);
    playerHUD = new QHBoxLayout;
    playerHUD->addWidget(player_sprite);
    playerHUD->addWidget(player_hitpoints);

    buttonsTab->addLayout(enemyHUD);
    buttonsTab->addLayout(playerHUD);

    buttonsTab->addLayout(buttonBox);
    buttonsTab->addWidget(consoleTxt);
    buttonsTab->addWidget(endingTxt);

    equipmentTab = new QVBoxLayout;
    equipmentTop = new QHBoxLayout;
    equipmentMid = new QHBoxLayout;
    equipmentBot = new QHBoxLayout;
    equipmentTop->addWidget(btn_necklace);
    equipmentTop->addWidget(btn_helmet);
    equipmentTop->addWidget(btn_backpack);

    equipmentMid->addWidget(btn_leftHand);
    equipmentMid->addWidget(btn_armor);
    equipmentMid->addWidget(btn_rightHand);

    equipmentBot->addWidget(btn_ring);
    equipmentBot->addWidget(btn_boots);
    equipmentBot->addWidget(btn_utility);

    backpackTab = new QHBoxLayout;
    backpackTab->addWidget(backpackSlot1);
    backpackTab->addWidget(backpackSlot2);
    backpackTab->addWidget(backpackSlot3);
    backpackTab->addWidget(backpackSlot4);

    backpackLine2 = new QHBoxLayout;
    backpackLine2->addWidget(backpackSlot5);
    backpackLine2->addWidget(backpackSlot6);
    backpackLine2->addWidget(backpackSlot7);
    backpackLine2->addWidget(backpackSlot8);

    equipmentTab->addWidget(equipament_label,Qt::AlignHCenter);
    equipmentTab->addLayout(equipmentTop);
    equipmentTab->addLayout(equipmentMid);
    equipmentTab->addLayout(equipmentBot);
    equipmentTab->addWidget(backpack_label,Qt::AlignHCenter);
    equipmentTab->addLayout(backpackTab);
    equipmentTab->addLayout(backpackLine2);
    equipmentTab->setSpacing(10);
    equipmentTab->setContentsMargins(0,0,0,0);

    mainUI->addLayout(skillsTab,Qt::AlignLeft);
    mainUI->addLayout(buttonsTab,Qt::AlignCenter);
    mainUI->addLayout(equipmentTab,Qt::AlignRight);

    setLayout(mainUI);
}

Interface::~Interface(){

}

void Interface::duel(){
    consoleTxt->setEnabled(true);
    endingTxt->setEnabled(false);
    _player->attack(_enemy);
    _enemy->attack(_player);
    QString _enemyhp = QString::number(_enemy->getLastDamage());
    QString _playerhp = QString::number(_player->getLastDamage());
    consoleTxt->append("\n"+_enemy->name() +" lost "+ _enemyhp +" due to your attack." );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    consoleTxt->append("You lost "+ _playerhp +" due to " +_enemy->name() +"  attack.");
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    int playerINT_hp = _player->getLifeLevel();
    int enemyINT_hp = _enemy->getLifeLevel();
    if(playerINT_hp <= 0){
        consoleTxt->setEnabled(false);
        endingTxt->setEnabled(true);
        endingTxt->setText("<h2><font color = red>\nYOU ARE DEAD\nGame Over</font></h2>");
        button_atq->setEnabled(false);
        button_def->setEnabled(false);
    }
    if (enemyINT_hp <= 0){
        consoleTxt->setEnabled(false);
        endingTxt->setEnabled(true);
        endingTxt->setText("<h2><font color = green>\nYOU WON\n</font></h2>");
        consoleTxt->clear();
        _enemy->newGameEnemy();
       _player->newGamePlayer();
    }
}

void Interface::healingPOT(){
    if(qtd_HpPot == 0){
        backpackSlot1->setEnabled(false);
    }
    int healthBefore = _player->getLifeLevel();
    _player->healPOT();
    int healthAfter = _player->getLifeLevel();
    int Calc = healthAfter - healthBefore;
    QString _playerhp = QString::number(Calc);
    QString _HpPOTcount = QString::number(qtd_HpPot);
    consoleTxt->append("\nYou healed yourself to " +_playerhp +"." );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    consoleTxt->append("\nUsing one of "+_HpPOTcount +" ultimate healing potions" );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    qtd_HpPot -= 1;
}

void Interface::healingUH(){
    if(qtd_UH == 0){
        backpackSlot2->setEnabled(false);
    }
    int healthBefore = _player->getLifeLevel();
    _player->healUH();
    int healthAfter = _player->getLifeLevel();
    int Calc = healthAfter - healthBefore;
    QString _playerhp = QString::number(Calc);
    QString _UHcount = QString::number(qtd_UH);
    consoleTxt->append("\nYou healed yourself to " +_playerhp +"." );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    consoleTxt->append("\nUsing one of "+_UHcount +" ultimate healing runes" );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    qtd_UH -= 1;
}

void Interface::manaPOT(){
    if(qtd_manaPot == 0){
        backpackSlot2->setEnabled(false);
    }
  //  int manaBefore = _player->getMana();
    _player->manaPot();
   // int manaAfter = _player->getMana();
   // int Calc = manaAfter - manaBefore;
    QString _manaPot = QString::number(qtd_manaPot);
    consoleTxt->append("\nUsing one of "+_manaPot +" mana potions" );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    qtd_manaPot -= 1;
}

void Interface::bigmanaPOT(){
    if(qtd_bigmanaPot == 0){
        backpackSlot2->setEnabled(false);
    }
   // int manaBefore = _player->getMana();
    _player->bigmanaPot();
   // int manaAfter = _player->getMana();
   // int Calc = manaAfter - manaBefore;
    QString _bigmanaPot = QString::number(qtd_bigmanaPot);
    consoleTxt->append("\nUsing one of "+_bigmanaPot +" big mana potions" );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    qtd_bigmanaPot -= 1;
}

void Interface::exura(){
    if(_player->getMana() == 0){
        button_exura->setEnabled(false);
    }
    button_exura->setEnabled(true);
    int healthBefore = _player->getLifeLevel();
    _player->magia_Exura();
    int healthAfter = _player->getLifeLevel();
    int Calc = healthAfter - healthBefore;
    QString _playerhp = QString::number(Calc);
    consoleTxt->append("\nYou said: Exura" );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
    consoleTxt->append("\nYou healed yourself to " +_playerhp +"." );
    consoleTxt->moveCursor(QTextCursor::End); // Move o ponteiro para o final da linha, para escrever o texto de baixo na mesma linha
}
