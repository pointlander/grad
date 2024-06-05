#include <stdint.h>
#include <stdio.h>

// V is a value
struct V {
    double X; // the value
    double D; // the derivative
};

// Continuation is a continuation
typedef uint8_t (*Continuation)(struct V *);
// Meta is a function that takes a continuation and return a continuation
typedef Continuation (*Meta)(Continuation);
// Unary is a unary function
typedef uint8_t (*Unary)(Continuation, struct V *);
// Binary is a binary function
typedef uint8_t (*Binary)(Continuation, struct V *, struct V *);

// Add adds two numbers
uint8_t Add(Continuation k, struct V *a, struct V *b) {
    printf("%f %f\n", a->X, b->X);
    struct V c = {a->X + b->X, 0};
    if (k(&c) == 1) {
        return 1;
    }
    a->D += c.D;
    b->D += c.D;
    return 0;
}

int main(int argc, char *argv[]) {
    // U converts a unary function into an operator
    typedef Meta (*UnaryOp)(Meta);
    UnaryOp U(Unary op) {
        Meta m(Meta a) {
                Continuation c(Continuation k) {
                        uint8_t cc(struct V *b)  {
                            return op(k, b);
                        }
                        return a(cc);
                }
                return c;
        }
        return m;
    }

    // B converts a binary function into an operator
    typedef Meta (*BinaryOp)(Meta, Meta);
    BinaryOp B(Binary op) {
        printf("here\n");
        Meta m(Meta a, Meta b) {
                printf("there\n");
                Continuation c(Continuation k) {
                        printf("over here\n");
                        uint8_t cc(struct V *aa) {
                            printf("aah\n");
                            uint8_t derivatives = 0;
                            uint8_t ccc(struct V *bb) {
                                printf("%f %f\n", aa->X, bb->X);
                                derivatives = op(k, aa, bb);
                                return derivatives;
                            }
                            b(ccc);
                            return derivatives;
                        }
                        printf("gah %p %p %p\n", a, cc, k);
                        Continuation x = a(cc);
                        printf("x\n");
                        return x;
                }
                return c;
        }
        return m;
    }

    uint8_t Panic(struct V *a) {
        printf("error!\n");
        return 0;
    }

    BinaryOp add = B(Add);
    struct V a = {1, 0};
    Continuation aa(Continuation k) {
        k(&a);
        return Panic;
    }
    struct V b = {2, 0};
    Continuation bb(Continuation k) {
        k(&b);
        return Panic;
    }
    Meta c = B(Add)(aa, bb);
    printf("1\n");
    uint8_t cc(struct V *x) {
        printf("%f\n", x->X);
        return 0;
    }
    printf("2 %p %p\n", c, cc);
    B(Add)(aa, bb)(cc);
    printf("3\n");
    return 0;
}
