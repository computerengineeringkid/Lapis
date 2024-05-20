#include <DirectXMath.h>

class Camera {
public:
    Camera(float aspectRatio);


    void SetProjectionMatrix(float aspectRatio);

    void UpdateViewMatrix();
    void Rotate(float pitch, float yaw);
    void Move(DirectX::XMVECTOR deltaPos);
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
};
