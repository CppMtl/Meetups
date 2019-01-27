#include <random>
using namespace std;
class Location {};
bool operator!=(Location, Location) { return true; }
class DrivingDirection {};
Location current_location() { return {}; }
Location query_destination() { return {}; }
DrivingDirection compute_direction(Location, Location) { return {}; }
bool all_clear(DrivingDirection) {
    static mt19937 prng{ random_device{}() };
    return uniform_int_distribution<int>{ 1, 4 }( prng ) != 1;
}
class CollisionRiskDetected{};
DrivingDirection drive(DrivingDirection current) {
   // ridiculously simplified
   if (all_clear(current)) return current;
   throw CollisionRiskDetected{};
}
void avoid_collision() {}
int main() {
   auto dest = query_destination();
   auto current = compute_direction(current_location(), dest);
   while(current_location() != dest)
      try {
         current = drive(current); // hum
      } catch(CollisionRiskDetected&) {
         avoid_collision();
      }
}
