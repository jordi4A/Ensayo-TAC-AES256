// T(key_expansion) = 1746
static void KeyExpansion(uint8_t* RoundKey, const uint8_t* Key) {

  unsigned i, j, k, Nb = 4, Nk = 8, Nr = 14;
  uint8_t tempa[4];
  
  // T(for) = T(cond) + T(i=0) + Iter*(T(C)+T(i++)+T(S))
  // T(for) = 1 + 1 + 8 * (2 + 2 + 28) = 258
  for (i = 0; i < Nk; ++i) {
    RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];   // 7
    RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];   // 7
    RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];   // 7
    RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];   // 7
  }

  // T(for) = 3 + 1 + 14*(3+2+101) = 1488
  for (i = Nk; i < Nb * (Nr + 1); ++i) {  
      k = (i - 1) * 4;          // 3
      tempa[0]=RoundKey[k + 0]; // 4
      tempa[1]=RoundKey[k + 1]; // 4
      tempa[2]=RoundKey[k + 2]; // 4
      tempa[3]=RoundKey[k + 3]; // 4

    // T(if) = T(cond) + Max {T(b_then), T(b_else)}
    // T(if) = 2 + 11 + 16 + 6 = 35
    if (i % Nk == 0) {
        const uint8_t u8tmp = tempa[0];
        tempa[0] = tempa[1];    // 3
        tempa[1] = tempa[2];    // 3
        tempa[2] = tempa[3];    // 3
        tempa[3] = u8tmp;       // 2

        tempa[0] = getSBoxValue(tempa[0]);  // 4
        tempa[1] = getSBoxValue(tempa[1]);  // 4
        tempa[2] = getSBoxValue(tempa[2]);  // 4
        tempa[3] = getSBoxValue(tempa[3]);  // 4

      tempa[0] = tempa[0] ^ Rcon[i/Nk];     // 6
    }

    // T(if) = 2 + 16 = 18
    if (i % Nk == 4){
        tempa[0] = getSBoxValue(tempa[0]);  // 4
        tempa[1] = getSBoxValue(tempa[1]);  // 4
        tempa[2] = getSBoxValue(tempa[2]);  // 4
        tempa[3] = getSBoxValue(tempa[3]);  // 4
    }

    j = i * 4;                              // 2
    k=(i - Nk) * 4;                         // 3

    RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];   // 6
    RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];   // 6
    RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];   // 6
    RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];   // 6
  }
}

// T(add_round_key) = 262
static void AddRoundKey(uint8_t round, state_t* state, const uint8_t* RoundKey) {
  uint8_t i,j;
  // T(for) = T(cond) + T(i=0) + Iter*(T(C)+T(i++)+T(S)
  // T(for) = 1 + 1 + 4*(1+2+62) = 262
  for (i = 0; i < 4; ++i) {
    // T(for) = 1 + 1 + 4*(1+2+12) = 62
    for (j = 0; j < 4; ++j) {
      (*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];  // 3 + 2 + 7 = 12
    }
  }

}
// T(sub_bytes) = 198
static void SubBytes(state_t* state) {
  uint8_t i, j;

  // T(for) = T(cond) + T(i=0) + Iter*(T(C)+T(i++)+T(S)
  // (Tfor) = 1 + 1 + 4*(1+2+46) = 198
  for (i = 0; i < 4; ++i) {
    // T(for) = 1 + 1 +4*(1+2+8) = 46
    for (j = 0; j < 4; ++j) {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);    // 3 + 1 + 4 = 8
    }
  }
}

// T(shift_rows) = 88
static void ShiftRows(state_t* state) {
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];  // 4
  (*state)[0][1] = (*state)[1][1];  // 7
  (*state)[1][1] = (*state)[2][1];  // 7
  (*state)[2][1] = (*state)[3][1];  // 7
  (*state)[3][1] = temp;            // 4

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];  // 4
  (*state)[0][2] = (*state)[2][2];  // 7
  (*state)[2][2] = temp;            // 4

  temp           = (*state)[1][2];  // 4
  (*state)[1][2] = (*state)[3][2];  // 7
  (*state)[3][2] = temp;            // 4

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];  // 4
  (*state)[0][3] = (*state)[3][3];  // 7
  (*state)[3][3] = (*state)[2][3];  // 7
  (*state)[2][3] = (*state)[1][3];  // 7
  (*state)[1][3] = temp;            // 4
}

// T(mix_columns) = 334
static void MixColumns(state_t* state) {
  uint8_t i;
  uint8_t Tmp, Tm, t;

  // T(for) = T(cond) + T(i=0) + Iter*(T(C)+T(i++)+T(S)
  // (Tfor) = 1 + 1 + 4*(1+2+80) = 334
  for (i = 0; i < 4; ++i) {  
    t   = (*state)[i][0];                                                       // 4
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;   // 16
    Tm  = (*state)[i][0] ^ (*state)[i][1] ;                                     // 8
    Tm = xtime(Tm);                                                             // 2
    (*state)[i][0] ^= Tm ^ Tmp ;                                                // 6
    Tm  = (*state)[i][1] ^ (*state)[i][2] ;                                     // 8
    Tm = xtime(Tm);                                                             // 2  
    (*state)[i][1] ^= Tm ^ Tmp ;                                                // 6
    Tm  = (*state)[i][2] ^ (*state)[i][3] ;                                     // 8
    Tm = xtime(Tm);                                                             // 2
    (*state)[i][2] ^= Tm ^ Tmp ;                                                // 6
    Tm  = (*state)[i][3] ^ t ;                                                  // 5
    Tm = xtime(Tm);                                                             // 5
    (*state)[i][3] ^= Tm ^ Tmp ;                                                // 2
  }
}