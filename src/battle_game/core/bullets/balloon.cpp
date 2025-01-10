#include "battle_game/core/bullets/balloon.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"
#include <glm/gtx/rotate_vector.hpp> 
// #include <cmath>

namespace battle_game::bullet {
BalloonBullet::BalloonBullet(GameCore *core,
                             uint32_t id,
                             uint32_t unit_id,
                             uint32_t player_id,
                             glm::vec2 position,
                             float rotation,
                             float damage_scale,
                             glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity) {
}

void BalloonBullet::Render() {
  SetTransformation(position_, rotation_, glm::vec2{size_});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

void BalloonBullet::Update() {
  // 更新位置，模拟弯曲前进
  
  float wave_frequency = 30.0f;  // 波浪频率
  float wave_amplitude = 0.8f;  // 波浪振幅

  // 使用正弦函数来调整速度，使其摆动
  float wave_offset = wave_amplitude * std::sin(wave_frequency * time_alive_);
  glm::vec2 wave_velocity = glm::rotate(velocity_, wave_offset);

  position_ += wave_velocity * kSecondPerTick;

  // 更新大小和攻击力
  size_ *= std::pow(0.2f, kSecondPerTick);
  damage_scale_ *= std::pow(0.2f, kSecondPerTick);

  // 更新存活时间
  time_alive_ += kSecondPerTick;
  if (time_alive_ > 1.5f) {
    game_core_->PushEventRemoveBullet(id_);
    return;
  }

  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f);
      should_die = true;
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

BalloonBullet::~BalloonBullet() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet