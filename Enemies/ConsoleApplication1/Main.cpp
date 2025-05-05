#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(1280, 720), "Ben 10 Game");

Vector2f Normalize(Vector2f vector) {
    float mag = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
    if (mag == 0) return Vector2f(0.f, 0.f);
    return Vector2f(vector.x / mag, vector.y / mag);
}

float Distance(Vector2f v1, Vector2f v2) {
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

float deltaTime;

Sprite player;
RectangleShape playerCollider;

Texture idleAnimation;
Texture walkAnimation;
Texture enemy1SpriteSheet;
Texture bossTexture;

struct Bullet {
    CircleShape shape;
    Vector2f velocity;
    int damage = 35;

    void Update() {
        shape.move(velocity * deltaTime);
    }
};

vector<Bullet> bullets;

struct Enemies {
    Sprite shape;
    RectangleShape attackBox;
    Vector2f velocity;
    int health = 100;
    float speed = 5;
    int animationRow = 2;
    int animationColumn = 0;
    float bulletTimer = 0.f;
    float bulletCooldown = 1.21f;
    int scale = 2;
    int type = 1; // 1 = melee, 2 = ranged
    float attackCounter = 0;
    float attackTime = 1;
    bool isAttacking = false;
    bool isDead = false;
    bool isHit = false;

    void Start() {
        shape.setTexture(enemy1SpriteSheet);
        attackBox.setSize(Vector2f(125, 125));
        attackBox.setOrigin(attackBox.getLocalBounds().width / 2, attackBox.getLocalBounds().height / 2);
        attackBox.setFillColor(Color::Yellow);
        shape.setOrigin(16, 16);
        shape.setScale(scale, scale);
    }

    void Update() {
        attackBox.setPosition(shape.getPosition());
        CalculateMovementDirection();
        if (!isAttacking)
            shape.move(velocity);
        AnimationHandler();
        if (type == 1)
            AttackPlayer();
        if (health <= 0)
            isDead = true;
    }

    void CalculateMovementDirection() {
        velocity = Normalize(player.getPosition() - shape.getPosition()) * speed;
    }

    void AnimationHandler() {
        animationRow = isAttacking ? 1 : 2;
        animationColumn = (animationColumn + 1) % (isAttacking ? 7 : 8);
        shape.setTextureRect(IntRect(animationColumn * 32, animationRow * 32, 32, 32));
        shape.setScale((shape.getPosition().x > player.getPosition().x) ? -scale : scale, scale);
    }

    void AttackPlayer() {
        if (attackBox.getGlobalBounds().intersects(playerCollider.getGlobalBounds())) {
            isAttacking = true;
            attackCounter += deltaTime;
            if (attackCounter >= attackTime) {
                attackCounter = 0;
                cout << "HIT" << endl;
            }
        }
        else {
            isAttacking = false;
        }
    }
};

struct TheBoss {
    Sprite shape;
    RectangleShape attackBox;
    Vector2f velocity;
    int health;
    int maxHealth;
    int damage;
    int deathCount = 0;
    float speed = 3.5f;
    int scale = 4;
    float attackRange = 200.f;
    float attackCooldown = 0.f;
    float attackTimer = 1.5f;
    bool isActive = true;
    bool isAttacking = false;
    bool isDead = false;
    int animationRow = 0;
    int animationColumn = 0;

    void Start(int generation = 0) {
        shape.setTexture(bossTexture);
        shape.setOrigin(32, 32);
        shape.setScale(scale, scale);

        // Scaling stats
        switch (generation) {
        case 0:
            maxHealth = 1200;
            damage = 25;
            break;
        case 1:
            maxHealth = 1300;
            damage = 30;
            break;
        case 2:
            maxHealth = 1350;
            damage = 35;
            break;
        }
        health = maxHealth;

        attackBox.setSize(Vector2f(200, 200));
        attackBox.setOrigin(attackBox.getLocalBounds().width / 2, attackBox.getLocalBounds().height / 2);
        attackBox.setFillColor(Color(255, 0, 0, 100));
    }

    void Update() {
        if (!isActive) return;

        attackBox.setPosition(shape.getPosition());
        ChasePlayer();
        AttackHandler();
        AnimationHandler();

        if (health <= 0) {
            deathCount++;
            if (deathCount < 3) {
                health = maxHealth; // Resurrect
            }
            else {
                isDead = true;
                isActive = false;
            }
        }
    }

    void ChasePlayer() {
        velocity = Normalize(player.getPosition() - shape.getPosition()) * speed;
        if (Distance(shape.getPosition(), player.getPosition()) > attackRange) {
            shape.move(velocity * deltaTime * 60.f);
        }
    }

    void AttackHandler() {
        if (Distance(shape.getPosition(), player.getPosition()) <= attackRange) {
            attackCooldown += deltaTime;
            if (attackCooldown >= attackTimer) {
                isAttacking = true;
                attackCooldown = 0.f;
                cout << "BOSS ATTACK! Damage: " << damage << endl;
            }
        }
        else {
            isAttacking = false;
        }
    }

    void AnimationHandler() {
        animationRow = isAttacking ? 1 : 0;
        animationColumn = (animationColumn + 1) % (isAttacking ? 7 : 8);
        shape.setTextureRect(IntRect(animationColumn * 64, animationRow * 64, 64, 64));
        shape.setScale((shape.getPosition().x > player.getPosition().x) ? -scale : scale, scale);
    }
};

vector<Enemies> enemies;
vector<TheBoss> bosses;

void FireAtPlayer(Enemies& enemy, vector<Bullet>& bullets, float bulletSpeed) {
    if (enemy.type != 2 || enemy.isDead) return;

    enemy.bulletTimer += deltaTime;
    if (enemy.bulletTimer >= enemy.bulletCooldown) {
        Bullet bullet;
        bullet.shape.setRadius(5.f);
        bullet.shape.setFillColor(Color::Yellow);
        bullet.shape.setPosition(enemy.shape.getPosition());
        bullet.velocity = Normalize(player.getPosition() - enemy.shape.getPosition()) * bulletSpeed;
        bullets.push_back(bullet);
        enemy.bulletTimer = 0.f;
    }
}

void UpdateBullets(vector<Bullet>& bullets, float deltaTime) {
    for (auto& bullet : bullets) {
        bullet.Update();
    }
}

void Start();
void Update();
void Draw();
void Collision();
void CalculateAimDirection();
void Shooting();
void ZombieHandler();
void BossHandler();
void SpawnEnemies();

int main() {
    Start();
    Clock clock;
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Update();
        Draw();
    }
    return 0;
}

void Start() {
    idleAnimation.loadFromFile("PlayerAnimations/Idle.png");
    walkAnimation.loadFromFile("PlayerAnimations/Walk.png");
    enemy1SpriteSheet.loadFromFile("EnemiesAnimations/Enemies.png");
    bossTexture.loadFromFile("BossAnimations/Boss.png"); // Add your boss texture
}

void Update() {
    CalculateAimDirection();
    Shooting();
    Collision();
    SpawnEnemies();
    ZombieHandler();
    BossHandler();

    for (Enemies& enemy : enemies) {
        FireAtPlayer(enemy, bullets, 300.f);
    }
    UpdateBullets(bullets, deltaTime);
}

void Collision() {
    // Bullet collisions
    for (int i = 0; i < bullets.size(); i++) {
        if (playerCollider.getGlobalBounds().intersects(bullets[i].shape.getGlobalBounds())) {
            bullets.erase(bullets.begin() + i);
            continue;
        }

        // Check enemy collisions
        for (int j = 0; j < enemies.size(); j++) {
            if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
                enemies[j].health -= bullets[i].damage;
                bullets.erase(bullets.begin() + i);
                break;
            }
        }

        // Check boss collisions
        for (int j = 0; j < bosses.size(); j++) {
            if (bosses[j].isActive && bullets[i].shape.getGlobalBounds().intersects(bosses[j].shape.getGlobalBounds())) {
                bosses[j].health -= bullets[i].damage;
                bullets.erase(bullets.begin() + i);
                break;
            }
        }
    }

    // Boss attack collision
    for (auto& boss : bosses) {
        if (boss.isAttacking && boss.attackBox.getGlobalBounds().intersects(playerCollider.getGlobalBounds())) {
            cout << "Player hit by boss!" << endl;
            // Add player damage logic here
        }
    }
}

void SpawnEnemies() {}
  
void ZombieHandler() {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].isDead) {
            enemies.erase(enemies.begin() + i);
            continue;
        }
        enemies[i].Update();
    }
}

void BossHandler() {
    for (int i = 0; i < bosses.size(); ) {
        if (bosses[i].isDead) {
            // On death, spawn two new bosses if not final generation
            if (bosses[i].deathCount < 3) {
                TheBoss newBoss1, newBoss2;
                Vector2f pos = bosses[i].shape.getPosition();

                newBoss1.Start(bosses[i].deathCount);
                newBoss1.shape.setPosition(pos.x - 100, pos.y - 100);

                newBoss2.Start(bosses[i].deathCount);
                newBoss2.shape.setPosition(pos.x + 100, pos.y + 100);

                bosses.push_back(newBoss1);
                bosses.push_back(newBoss2);
            }
            bosses.erase(bosses.begin() + i);
        }
        else {
            bosses[i].Update();
            i++;
        }
    }
}

void Draw() {
    window.clear();

    // Draw bullets
    for (auto& bullet : bullets) {
        window.draw(bullet.shape);
    }

    // Draw enemies
    for (auto& enemy : enemies) {
        window.draw(enemy.attackBox);
        window.draw(enemy.shape);
    }

    // Draw bosses
    for (auto& boss : bosses) {
        if (boss.isActive) {
            window.draw(boss.attackBox);
            window.draw(boss.shape);
        }
    }

    window.display();
}

void CalculateAimDirection() {}
void Shooting() {}

