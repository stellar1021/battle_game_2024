#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class BalloonBullet : public Bullet {
 public:
  BalloonBullet(GameCore *core,
                uint32_t id,
                uint32_t unit_id,
                uint32_t player_id,
                glm::vec2 position,
                float rotation,
                float damage_scale,
                glm::vec2 velocity);
  ~BalloonBullet() override;
  void Render() override;
  void Update() override;

 private:
  glm::vec2 velocity_{};
  float size_{0.2f};  // 初始大小为普通子弹的2倍
  float time_alive_{0.0f};  // 存活时间
};
}  // namespace battle_game::bullet