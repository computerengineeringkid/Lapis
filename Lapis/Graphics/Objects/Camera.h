#include <DirectXMath.h>

class Camera {
public:
    Camera(float aspectRatio);


    void SetProjectionMatrix(float aspectRatio);
    void ProcessInput(float deltaTime);
    void UpdateViewMatrix();
    void Update(float deltaTime);
    void Rotate(float dpitch, float dyaw);
    void Move(DirectX::XMVECTOR direction);
    DirectX::XMMATRIX GetViewMatrix() const {
        return viewMatrix;
    }

    DirectX::XMMATRIX GetProjectionMatrix() const {
        return projMatrix;
    }

private:
    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projMatrix;
    DirectX::XMVECTOR position;
    DirectX::XMVECTOR lookAt;
    DirectX::XMVECTOR up;

    float pitch = 0.0f; // Cumulative pitch angle
    float yaw = 0.0f;   // Cumulative yaw angle
};
