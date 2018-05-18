#pragma once

class CPixelShader {
    ID3D11PixelShader*   ps = nullptr;

public:
    ~CPixelShader() { destroy(); }
    bool create(const std::string& filename, const std::string& fn_entry_name);
    void destroy();
    void activate() const;
};
