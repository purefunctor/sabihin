module TextEncoder = TextEncoder;
module TextDecoder = TextDecoder;
module WebCrypto = WebCrypto;

module ClientRandom = ClientRandom;
module Salt = Salt;

module DerivedSecrets: DerivedSecretsInternal.S = DerivedSecretsInternal;
module MasterKey: MasterKeyInternal.S = MasterKeyInternal;
module ProtectionKeys: ProtectionKeysInternal.S = ProtectionKeysInternal;
module VerificationKeys: VerificationKeysInternal.S = VerificationKeysInternal;
module EphemeralKey: EphemeralKeyInternal.S = EphemeralKeyInternal;
module Operations: OperationsInternal.S = OperationsInternal;
