#pragma once

HRESULT CompileShaderFromFile(
    const char* szFileName,
    const char* szEntryPoint,
    const char* szShaderModel,
    ID3DBlob** ppBlobOut);
